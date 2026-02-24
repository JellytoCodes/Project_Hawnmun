// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Controllers/HawnmunPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Input/HawnmunEnhancedInputComponent.h"

AHawnmunPlayerController::AHawnmunPlayerController()
{
	PlayerId = FGenericTeamId(0);
}

FGenericTeamId AHawnmunPlayerController::GetGenericTeamId() const
{
	return PlayerId;
}

void AHawnmunPlayerController::EnableDefaultMappingContext()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);	
	}
}

void AHawnmunPlayerController::DisableDefaultMappingContext()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(PlayerContext);	
	}
}

void AHawnmunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = GetPawn<APawn>();

	check(PlayerContext);
	check(InputConfig);

	EnableDefaultMappingContext();
}

void AHawnmunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UHawnmunEnhancedInputComponent* HawnmunInputComponent = CastChecked<UHawnmunEnhancedInputComponent>(InputComponent);

	if (HawnmunInputComponent)
	{
		HawnmunInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		HawnmunInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		HawnmunInputComponent->BindAction(SwitchTargetAction, ETriggerEvent::Started, this, &ThisClass::SwitchTarget);

		HawnmunInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void AHawnmunPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC()->HasMatchingGameplayTag(HawnmunGameplayTags::Input_Key_Locked)) return;
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
	if (ControlledPawn == nullptr) return;

	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHawnmunPlayerController::SwitchTarget(const FInputActionValue& InputActionValue)
{
	const float SwitchValue = InputActionValue.Get<float>();
	const FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetPawn(),
		SwitchValue > 0.f ? HawnmunGameplayTags::Event_SwitchTarget_Left : HawnmunGameplayTags::Event_SwitchTarget_Right,
		Data);
}

void AHawnmunPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC())
		GetASC()->AbilityInputTagPressed(InputTag);
}

void AHawnmunPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
		GetASC()->AbilityInputTagReleased(InputTag);
}

void AHawnmunPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//if (GetASC())
		//GetASC()->AbilityInputTagHeld(InputTag);
}

UHawnmunAbilitySystemComponent* AHawnmunPlayerController::GetASC()
{
	if (HawnmunAbilitySystemComponent == nullptr)
	{
		HawnmunAbilitySystemComponent = Cast<UHawnmunAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HawnmunAbilitySystemComponent.Get();
}
