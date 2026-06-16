// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunWildBoar.h"

#include "BrainComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "HawnmunGameplayTags.h"
#include "HUD/HawnmunHUD.h"
#include "HUD/WidgetController/CinematicWidgetController.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "Engine/TargetPoint.h"
#include "GameMode/HawnmunGameModeBase.h"

bool AHawnmunWildBoar::TryPhaseTransition()
{
	return false;
}

void AHawnmunWildBoar::Die()
{
	StopWildBoarCombatLogic(TEXT("WildBoarDied"));

	Super::Die();

	if (AHawnmunPlayer* Player = Cast<AHawnmunPlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->ProcessLooting(this);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD());
		checkf(HUD, TEXT("Die failed: HUD is null."));

		HUD->ShowCinematicWidget();
		HUD->GetCinematicSlotWidgetController(FWidgetControllerParams())->RequestPlayCinematic(HawnmunGameplayTags::Cinematic_WildBoar_Defeat, this);
	}
}

void AHawnmunWildBoar::ResumeEnemyAI_Implementation()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("ResumeEnemyAI failed: ASC is null on %s."), *GetNameSafe(this));

	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));

	checkf(GameMode, TEXT("ResumeEnemyAI failed: HawnmunGameModeBase is null."));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Dead))
	{
		MovePlayerToDefeatTransitionPoint();
		GameMode->ChangeBGMByTag(HawnmunGameplayTags::BGM_CombatField_Default, 0.5f, 0.f);
		return;
	}

	GameMode->ChangeBGMByTag(HawnmunGameplayTags::BGM_WildBoar_Battle, 0.5f, 0.f);
}

void AHawnmunWildBoar::StopWildBoarCombatLogic(const FString& Reason)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->CancelAllAbilities();
	}

	if (UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		AnimInstance->Montage_Stop(0.f);
	}

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();

		if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
		{
			BrainComp->StopLogic(Reason);
		}
	}

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->StopMovementImmediately();
	}
}

void AHawnmunWildBoar::MovePlayerToDefeatTransitionPoint() const
{
	if (!DefeatTransitionPoint) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	const FVector TargetLocation = DefeatTransitionPoint->GetActorLocation();
	const FRotator TargetRotation = DefeatTransitionPoint->GetActorRotation();

	PlayerPawn->SetActorLocationAndRotation(
		TargetLocation,
		TargetRotation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);

	if (UHawnmunDungeonSubsystem* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		DungeonSubsystem->SaveCheckpoint(PlayerPawn->GetActorTransform());
	}
}