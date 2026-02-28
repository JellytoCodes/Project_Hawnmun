// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Actors/FoxFireDropActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
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
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);

			const bool bIsRolling = TargetASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Rolling);

			FGameplayEventData EventData;
			EventData.Instigator = CombatDamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
			EventData.Target = TargetASC->GetAvatarActor();

			if (bIsRolling)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatDamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
			}
			else
			{
				CombatDamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UHawnmunFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
			}
		}
	}
}

void AFoxFireDropActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}
