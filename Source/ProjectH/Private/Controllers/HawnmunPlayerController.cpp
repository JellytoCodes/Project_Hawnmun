// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Controllers/HawnmunPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Actors/InteractProp.h"
#include "Characters/HawnmunPlayer.h"
#include "HUD/HawnmunHUD.h"
#include "Input/HawnmunEnhancedInputComponent.h"
#include "Controllers/HawnmunPlayerCameraManager.h"

#pragma region Core And Initialization
AHawnmunPlayerController::AHawnmunPlayerController()
{
	PlayerId = FGenericTeamId(0);
	PlayerCameraManagerClass = AHawnmunPlayerCameraManager::StaticClass();
}

FGenericTeamId AHawnmunPlayerController::GetGenericTeamId() const
{
	return PlayerId;
}

void AHawnmunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = GetPawn<APawn>();

	check(PlayerContext);
	check(InputConfig);

	EnableDefaultMappingContext();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;

	HoldBlackScreen();

	GetWorldTimerManager().SetTimer(InitialFadeInTimerHandle,this,&ThisClass::StartInitialFadeIn, 0.5f, false);
}

void AHawnmunPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = InPawn;
    HawnmunAbilitySystemComponent = nullptr;
}

void AHawnmunPlayerController::EnableDefaultMappingContext() const
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(PlayerContext, 0);
}

void AHawnmunPlayerController::DisableDefaultMappingContext() const
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->RemoveMappingContext(PlayerContext);
}

void AHawnmunPlayerController::SetInLobby(const bool InState)
{
	bInLobby = InState;
}

void AHawnmunPlayerController::RestoreGameplayInput()
{
	ControlledPawn = GetPawn();

    EnableDefaultMappingContext();

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);

    bShowMouseCursor = false;

    if (AHawnmunHUD* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
    {
        HawnmunHUD->ShowOverlay();
    }

    if (PlayerCameraManager)
    {
        PlayerCameraManager->StartCameraFade(1.f, 0.f, 0.35f, FLinearColor::Black, false, false);
    }
}
#pragma endregion

#pragma region Input Setup And Bindings
void AHawnmunPlayerController::SetCinematicInputBlocked(bool bBlocked)
{
	bCinematicInputBlocked = bBlocked;

	SetIgnoreMoveInput(bBlocked);
	SetIgnoreLookInput(bBlocked);

	if (UAbilitySystemComponent* ASC = GetASC())
	{
		bBlocked ? 
		ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::Input_Key_Locked, 1) :
		ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::Input_Key_Locked, 0);
	}

}

void AHawnmunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UHawnmunEnhancedInputComponent* HawnmunInputComponent = CastChecked<UHawnmunEnhancedInputComponent>(InputComponent))
	{
		HawnmunInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		HawnmunInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		HawnmunInputComponent->BindAction(SwitchTargetAction, ETriggerEvent::Started, this, &ThisClass::SwitchTarget);

		HawnmunInputComponent->BindAction(AttributeWidgetAction, ETriggerEvent::Started, this, &ThisClass::AttributeWidgetSwitching);
		HawnmunInputComponent->BindAction(InventoryWidgetAction, ETriggerEvent::Started, this, &ThisClass::InventoryWidgetSwitching);
		HawnmunInputComponent->BindAction(InteractableAction, ETriggerEvent::Started, this, &ThisClass::InteractableByKey);
		HawnmunInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ThisClass::RequestCancel);

		HawnmunInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased);
	}
}
#pragma endregion

#pragma region Movement And Looking
void AHawnmunPlayerController::Move(const FInputActionValue& InputActionValue)
{
	UHawnmunAbilitySystemComponent* ASC = GetASC();
	checkf(ASC, TEXT("Move failed: ASC is null for controller %s."), *GetNameSafe(this));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Input_Key_Locked) || bCinematicInputBlocked) return;
	if (ControlledPawn == nullptr) return;

	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}	
}

void AHawnmunPlayerController::Look(const FInputActionValue& InputActionValue)
{
	if (ControlledPawn == nullptr || bCinematicInputBlocked) return;

	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	
	if (LookAxisVector.Y != 0.f)
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
}

void AHawnmunPlayerController::SwitchTarget(const FInputActionValue& InputActionValue)
{
	const float SwitchValue = InputActionValue.Get<float>();
	const FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetPawn(),
		SwitchValue > 0.f ? HawnmunGameplayTags::Event_SwitchTarget_Left : HawnmunGameplayTags::Event_SwitchTarget_Right, 
		Data);
}
#pragma endregion

#pragma region UI And Widget Management
void AHawnmunPlayerController::SetIsInteractable(const bool InIsInteractable, const EInteractableList InList, AActor* TargetActor)
{
	bIsInteractable = InIsInteractable;
	InteractableList = InList;
	CurrentInteractableTarget = TargetActor;
}

void AHawnmunPlayerController::CloseCurrentWidget()
{
	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
	{
		if (InteractableList == EInteractableList::AttributeWidget)
		{
			HawnmunHUD->HideAttribute();
		}
		else if (InteractableList == EInteractableList::InventoryWidget)
		{
			HawnmunHUD->HideInventory();
		}
		else if (InteractableList == EInteractableList::SaveWidget)
		{
			HawnmunHUD->HideSaveSlot();
		}
		else if (InteractableList == EInteractableList::DungeonMapWidget)
		{
			HawnmunHUD->HideDungeonMap();
		}
		else if (InteractableList == EInteractableList::PauseWidget)
		{
			HawnmunHUD->HidePauseMenu();
		}

		InteractableList = EInteractableList::None;
		SetUIModeEnabled(false);
	}
}

void AHawnmunPlayerController::AttributeWidgetSwitching()
{
	if (bCinematicInputBlocked) return;

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
	{
		if (InteractableList == EInteractableList::None)
		{
			HawnmunHUD->ShowAttribute();
			InteractableList = EInteractableList::AttributeWidget;
			SetUIModeEnabled(true);
		}
		else if (InteractableList == EInteractableList::AttributeWidget)
		{
			CloseCurrentWidget();
		}
	}
}

void AHawnmunPlayerController::InventoryWidgetSwitching()
{
	if (bCinematicInputBlocked) return;

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
	{
		if (InteractableList == EInteractableList::None)
		{
			HawnmunHUD->ShowInventory();
			InteractableList = EInteractableList::InventoryWidget;
			SetUIModeEnabled(true);
		}
		else if (InteractableList == EInteractableList::InventoryWidget)
		{
			CloseCurrentWidget();
		}
	}
}

void AHawnmunPlayerController::DungeonMapWidgetSwitching()
{
	if (bCinematicInputBlocked) return;

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
	{
		if (InteractableList == EInteractableList::None)
		{
			HawnmunHUD->ShowDungeonMap();
			InteractableList = EInteractableList::DungeonMapWidget;
			SetUIModeEnabled(true);
		}
	}
}

void AHawnmunPlayerController::InteractableByKey()
{
	if (bIsInteractable == false || bCinematicInputBlocked) return;

	auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD());

	switch (InteractableList)
	{
	case EInteractableList::None:
		break;
	case EInteractableList::AttributeWidget:
		HawnmunHUD->ShowAttribute();
		SetUIModeEnabled(true);
		break;
	case EInteractableList::InventoryWidget:
		HawnmunHUD->ShowInventory();
		SetUIModeEnabled(true);
		break;
	case EInteractableList::SaveWidget:
		HawnmunHUD->ShowSaveSlot();
		SetUIModeEnabled(true);
		break;
	case EInteractableList::DungeonMapWidget:
		HawnmunHUD->ShowDungeonMap();
		SetUIModeEnabled(true);
		break;
	case EInteractableList::Loot:
		if (AHawnmunPlayer* MainPlayer = GetPawn<AHawnmunPlayer>())
		{
			MainPlayer->ProcessLooting(CurrentInteractableTarget.Get());
		}
		break;
	case EInteractableList::Interact:
		if (AInteractProp* Prop = Cast<AInteractProp>(CurrentInteractableTarget.Get()))
		{
			Prop->TriggerInteract(GetPawn());
		}
		break;
	default:
		break;
	}
}

void AHawnmunPlayerController::RequestCancel()
{
	if (bCinematicInputBlocked) return;

	if (InteractableList == EInteractableList::AttributeWidget ||
		InteractableList == EInteractableList::InventoryWidget ||
		InteractableList == EInteractableList::SaveWidget ||
		InteractableList == EInteractableList::DungeonMapWidget ||
		InteractableList == EInteractableList::PauseWidget)
	{
		CloseCurrentWidget();
		return;
	}

	if (InteractableList == EInteractableList::None)
	{
		if (auto* HawnmunHUD = Cast<AHawnmunHUD>(GetHUD()))
		{
			HawnmunHUD->ShowPauseMenu();

			InteractableList = EInteractableList::PauseWidget;
			SetUIModeEnabled(true);
		}
	}
}

void AHawnmunPlayerController::SetUIModeEnabled(bool bEnable)
{
	SetPause(bEnable);
	bShowMouseCursor = bEnable;

	if (bEnable)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}
#pragma endregion

#pragma region Ability System Input
void AHawnmunPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (bCinematicInputBlocked) return;

	UHawnmunAbilitySystemComponent* ASC = GetASC();
	checkf(ASC, TEXT("AbilityInputTagPressed failed: ASC is null for controller %s."), *GetNameSafe(this));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Input_Key_Locked) || bInLobby) return;

	ASC->AbilityInputTagPressed(InputTag);
}

void AHawnmunPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (bCinematicInputBlocked) return;

	UHawnmunAbilitySystemComponent* ASC = GetASC();
	checkf(ASC, TEXT("AbilityInputTagReleased failed: ASC is null for controller %s."), *GetNameSafe(this));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Input_Key_Locked) || bInLobby) return;
	ASC->AbilityInputTagReleased(InputTag);
}

UHawnmunAbilitySystemComponent* AHawnmunPlayerController::GetASC()
{
	if (HawnmunAbilitySystemComponent == nullptr)
	{
		HawnmunAbilitySystemComponent = Cast<UHawnmunAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HawnmunAbilitySystemComponent.Get();
}
#pragma endregion

#pragma region Screen Transition
void AHawnmunPlayerController::HoldBlackScreen()
{
	checkf(PlayerCameraManager, TEXT("HoldBlackScreen: PlayerCameraManager is null"));
	PlayerCameraManager->SetManualCameraFade(1.f, FLinearColor::Black, false);
}

void AHawnmunPlayerController::FadeInFromBlack(float Duration)
{
	checkf(PlayerCameraManager, TEXT("FadeInFromBlack: PlayerCameraManager is null"));
	PlayerCameraManager->StartCameraFade(1.f, 0.f, Duration, FLinearColor::Black, false, false);
}

void AHawnmunPlayerController::StartInitialFadeIn()
{
	FadeInFromBlack(2.f);
}
#pragma endregion