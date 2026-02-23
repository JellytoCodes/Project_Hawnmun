// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunGameplayEffectContext.h"
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

bool UHawnmunFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsBlockedHit();
	}
	return false;
}

bool UHawnmunFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsCriticalHit();
	}
	return false;
}

bool UHawnmunFunctionLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UHawnmunFunctionLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UHawnmunFunctionLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UHawnmunFunctionLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UHawnmunFunctionLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *EffectContext->GetDamageType();
	}
	return FGameplayTag();
}

FVector UHawnmunFunctionLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDeathImpulse();
	}
	return FVector();
}

FVector UHawnmunFunctionLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetKnockbackForce();
	}
	return FVector();
}

bool UHawnmunFunctionLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsRadialDamage();
	}
	return false;
}

float UHawnmunFunctionLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UHawnmunFunctionLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UHawnmunFunctionLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHawnmunGameplayEffectContext* EffectContext = static_cast<const FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageOrigin();
	}
	return FVector();
}

void UHawnmunFunctionLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UHawnmunFunctionLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsCriticalHit(bInIsCriticalHit);
}

void UHawnmunFunctionLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
}

void UHawnmunFunctionLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetDebuffDamage(InDebuffDamage);
}

void UHawnmunFunctionLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetDebuffDuration(InDebuffDuration);
}

void UHawnmunFunctionLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetDebuffFrequency(InDebuffFrequency);
}

void UHawnmunFunctionLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		EffectContext->SetDamageType(DamageType);
	}
}

void UHawnmunFunctionLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetDeathImpulse(InImpulse);
}

void UHawnmunFunctionLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetKnockbackForce(InForce);
}

void UHawnmunFunctionLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
		if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsRadialDamage(bInIsRadialDamage);
}

void UHawnmunFunctionLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
}

void UHawnmunFunctionLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
}

void UHawnmunFunctionLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin)
{
	if (FHawnmunGameplayEffectContext* EffectContext = static_cast<FHawnmunGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
}

void UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, 
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
		FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UHawnmunFunctionLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UHawnmunFunctionLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;	
	else
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
}

void UHawnmunFunctionLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams,
	FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;	
	else
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
}

void UHawnmunFunctionLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
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
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, HawnmunGameplayTags::Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, HawnmunGameplayTags::Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, HawnmunGameplayTags::Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, HawnmunGameplayTags::Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
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
