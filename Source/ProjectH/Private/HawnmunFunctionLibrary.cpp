// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Engine/OverlapResult.h"

UHawnmunAbilitySystemComponent* UHawnmunFunctionLibrary::NativeGetHawnmunASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UHawnmunAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UHawnmunFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd)) 
		ASC->AddLooseGameplayTag(TagToAdd);
}

void UHawnmunFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove)) 
		ASC->RemoveLooseGameplayTag(TagToRemove);
}

bool UHawnmunFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UHawnmunFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EHawnmunConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EHawnmunConfirmType::Yes : EHawnmunConfirmType::No;
}

bool UHawnmunFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

FGameplayEffectContextHandle UHawnmunFunctionLibrary::ApplyDamageEffect(FDamageEffectParams DamageEffectParams)
{
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);

	const FGameplayTag HitReactTag = HawnmunGameplayTags::Event_HitReact;
	DamageEffectParams.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(HitReactTag.GetSingleTagContainer());

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

void UHawnmunFunctionLibrary::GetLivePlayersWithinCircle(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, 
	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(ActorsToIgnore);
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;

		World->OverlapMultiByObjectType(
			Overlaps, 
			SphereOrigin, 
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), 
			SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UHawnmunFunctionLibrary::GetLivePlayersWithinLine(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, 
	const TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End)
{
	FCollisionQueryParams LineParams;

	LineParams.AddIgnoredActors(ActorsToIgnore);
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FHitResult> Overlaps;

		World->LineTraceMultiByObjectType(
			Overlaps,
			Start,
			End,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			LineParams);

		for (FHitResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}
