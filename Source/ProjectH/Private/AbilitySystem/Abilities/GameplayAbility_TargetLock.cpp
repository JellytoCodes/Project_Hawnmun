// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/Abilities/GameplayAbility_TargetLock.h"

#include "EnhancedInputSubsystems.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/HawnmunPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/HawnmunPlayerController.h"
#include "Characters/HawnmunEnemy.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void UGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryLockOnTarget();

	if (!CurrentLockedActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	InitTargetLockMovement();
	InitTargetLockMappingContext();
}

void UGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	AHawnmunEnemy* LockedActor = CurrentLockedActor;
	AHawnmunPlayer* Player = GetPlayerFromActorInfo();
	AHawnmunPlayerController* PlayerController = GetPlayerControllerFromActorInfo();

	if (!IsValid(LockedActor) || !IsValid(Player) || !IsValid(PlayerController) || !IsValid(DrawnTargetLockWidget))
	{
		CancelTargetLockAbility();
		return;
	}

	if (LockedActor->IsHidden() || !LockedActor->GetActorEnableCollision())
	{
		CancelTargetLockAbility();
		return;
	}

	const bool bIsDead = UHawnmunFunctionLibrary::NativeDoesActorHaveTag(LockedActor, HawnmunGameplayTags::State_Dead) || UHawnmunFunctionLibrary::NativeDoesActorHaveTag(Player, HawnmunGameplayTags::State_Dead);

	if (bIsDead)
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation = !UHawnmunFunctionLibrary::NativeDoesActorHaveTag(Player, HawnmunGameplayTags::State_Rolling);

	if (!bShouldOverrideRotation)
	{
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), LockedActor->GetActorLocation());
	LookAtRotation -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

	const FRotator CurrentControlRotation = PlayerController->GetControlRotation();
	const FRotator TargetRotation = FMath::RInterpTo(CurrentControlRotation, LookAtRotation, DeltaTime, TargetLockRotationInterpSpeed);

	PlayerController->SetControlRotation(FRotator(TargetRotation.Pitch, TargetRotation.Yaw, 0.f));
	Player->SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
}

void UGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AHawnmunEnemy*> ActorsOnLeft;
	TArray<AHawnmunEnemy*> ActorsOnRight;
	AHawnmunEnemy* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == HawnmunGameplayTags::Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	 AvailableActorsToLock.Empty();

	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerFromActorInfo(),
		GetPlayerFromActorInfo()->GetActorLocation(),
		GetPlayerFromActorInfo()->GetActorLocation() + GetPlayerFromActorInfo()->GetActorForwardVector() * TraceDistance,
		TraceBoxSize / 2.f,
		GetPlayerFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{

		if (AHawnmunEnemy* HitActor = Cast<AHawnmunEnemy>(TraceHit.GetActor()))
		{
			AvailableActorsToLock.AddUnique(HitActor);
		}
	}
}

AHawnmunEnemy* UGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AHawnmunEnemy*>& InAvailableActors)
{
	if (InAvailableActors.IsEmpty()) return nullptr;

	AHawnmunEnemy* NearestEnemy = nullptr;
	float ClosestDistanceSq = MAX_FLT; 
	const FVector PlayerLocation = GetPlayerFromActorInfo()->GetActorLocation();

	for (AHawnmunEnemy* Enemy : InAvailableActors)
	{
		if (!Enemy) continue;

		const float DistanceSq = (Enemy->GetActorLocation() - PlayerLocation).SizeSquared();

		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			NearestEnemy = Enemy;
		}
	}

	return NearestEnemy;
}

void UGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AHawnmunEnemy*>& OutActorsOnLeft, TArray<AHawnmunEnemy*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AHawnmunEnemy* AvailableActor : AvailableActorsToLock) 
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
            
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));
		DrawnTargetLockWidget = CreateWidget<UUserWidget>(GetPlayerControllerFromActorInfo(), TargetLockWidgetClass);
		check(DrawnTargetLockWidget);
		DrawnTargetLockWidget->AddToViewport();	
	}
}

void UGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}

	FVector TargetWorldLocation = CurrentLockedActor->GetActorLocation();

	if (const ACharacter* TargetCharacter = Cast<ACharacter>(CurrentLockedActor))
	{
		const float HalfHeight = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		// 중앙(44)에서 캡슐의 절반 높이의 50%(22)만큼 올려 총 66의 위치로 조정
		const float ZOffset = HalfHeight * 0.5f;
		TargetWorldLocation.Z += ZOffset;
	}

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		TargetWorldLocation, // 기존 CurrentLockedActor->GetActorLocation() 대신 계산된 위치 사용
		ScreenPosition,
		true);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (const USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			});
	}

	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetPlayerFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetPlayerFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget) DrawnTargetLockWidget->RemoveFromParent();

	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;
	CachedDefaultMaxWalkSpeed = 0;
}

void UGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f) GetPlayerFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
}

void UGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!CurrentActorInfo->PlayerController.Get()) return;

	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

AHawnmunPlayer* UGameplayAbility_TargetLock::GetPlayerFromActorInfo()
{
	if (!CachedPlayer.IsValid())
	{
		CachedPlayer = Cast<AHawnmunPlayer>(CurrentActorInfo->AvatarActor);	
	}
	return CachedPlayer.IsValid() ? CachedPlayer.Get() : nullptr;
}

AHawnmunPlayerController* UGameplayAbility_TargetLock::GetPlayerControllerFromActorInfo()
{
	if (!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<AHawnmunPlayerController>(CurrentActorInfo->PlayerController);	
	}
	return CachedPlayerController.IsValid() ? CachedPlayerController.Get() : nullptr;
}
