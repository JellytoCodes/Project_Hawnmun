// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Actors/FoxFireDropActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "Interfaces/PlayerInterface.h"
#include "NiagaraComponent.h"
#include "ProjectH/ProjectH.h"

AFoxFireDropActor::AFoxFireDropActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	Effect->SetAutoActivate(false);
}

void AFoxFireDropActor::ActiveFoxFireDrop(TArray<AActor*>& OutOverlappingActors, FDamageEffectParams CombatDamageEffectParams)
{
	UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(
		this,
		OutOverlappingActors,
		ActorsToIgnore,
		Radius,
		GetActorLocation());

	for (auto& Actor : OutOverlappingActors)
	{
		if (Actor->Implements<UPlayerInterface>())
		{
			CombatDamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			UHawnmunFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
		}
	}
}

void AFoxFireDropActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}
