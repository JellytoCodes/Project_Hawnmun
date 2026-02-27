// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/PortalBreathActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "Interfaces/PlayerInterface.h"
#include "NiagaraComponent.h"
#include "ProjectH/ProjectH.h"

APortalBreathActor::APortalBreathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	Effect->SetAutoActivate(false);
}

void APortalBreathActor::ActiveBreath(TArray<AActor*>& OutOverlappingActors, FDamageEffectParams CombatDamageEffectParams)
{
	const FVector StartLocation = GetActorLocation();
	const FRotator AttackRotation = GetActorRotation();
	
	const FVector ForwardVector = AttackRotation.Vector();
	const FVector AdjOrigin = StartLocation + (ForwardVector * BoxExtent.X);

	UHawnmunFunctionLibrary::GetLivePlayersWithinBox(
		this,
		OutOverlappingActors,
		ActorsToIgnore,
		BoxExtent,
		AttackRotation,
		AdjOrigin
	);

	for (auto& Actor : OutOverlappingActors)
	{
		if (Actor->Implements<UPlayerInterface>())
		{
			CombatDamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			UHawnmunFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
		}
	}
}

void APortalBreathActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}
