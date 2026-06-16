// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/PortalBreathActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "Interfaces/PlayerInterface.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectH/ProjectH.h"

APortalBreathActor::APortalBreathActor()
{
	PrimaryActorTick.bCanEverTick = false; 

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	WarningDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("WarningDecal"));
	WarningDecal->SetupAttachment(RootComp);
	WarningDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); 
	WarningDecal->SetVisibility(false);

	PortalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalEffect"));
	PortalEffect->SetupAttachment(RootComp);
	PortalEffect->SetAutoActivate(false);

	BeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamEffect"));
	BeamEffect->SetupAttachment(RootComp);
	BeamEffect->SetAutoActivate(false);

	FoxHeadEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FoxHeadEffect"));
	FoxHeadEffect->SetupAttachment(RootComp);
	FoxHeadEffect->SetAutoActivate(false);
}

void APortalBreathActor::BeginPlay()
{
	Super::BeginPlay();

	check(WarningDecal);
	check(PortalEffect);
	check(BeamEffect);
	check(FoxHeadEffect);

	DecalMID = WarningDecal->CreateDynamicMaterialInstance();
	check(DecalMID);
}

void APortalBreathActor::ActivePortalBreath(const FDamageEffectParams& CombatDamageEffectParams)
{
	bIsActive = true;
	CachedDamageParams = CombatDamageEffectParams;

	PortalEffect->Deactivate();
	PortalEffect->Activate(true);

	BeamEffect->Deactivate();

	FoxHeadEffect->Deactivate();
	FoxHeadEffect->Activate(true);

	WarningDecal->SetVisibility(true);
	WarningDecal->DecalSize = FVector(BoxExtent.Z, BoxExtent.Y, BoxExtent.X);
	WarningDecal->SetRelativeLocation(FVector(BoxExtent.X, 0.f, 0.f));

	DecalMID->SetScalarParameterValue(FName("FillAmount"), 0.f);

	StartChargeAnimation();
}

void APortalBreathActor::CancelPortalBreath()
{
	bIsActive = false;

	WarningDecal->SetVisibility(false);
	DecalMID->SetScalarParameterValue(FName("FillAmount"), 0.f);

	PortalEffect->Deactivate();
	BeamEffect->Deactivate();
	FoxHeadEffect->Deactivate();

	CachedDamageParams = FDamageEffectParams();
}

void APortalBreathActor::UpdateWarningDecalProgress(float Progress)
{
	if (!bIsActive) return;

	DecalMID->SetScalarParameterValue(FName("FillAmount"), Progress);
}

void APortalBreathActor::FireBeam()
{
	if (!bIsActive) return;

	WarningDecal->SetVisibility(false);
	PortalEffect->Deactivate();

	BeamEffect->Deactivate();
	BeamEffect->Activate(true);

	OnBeamFired();
	ApplyDamage();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor(),
		HawnmunGameplayTags::Event_Combat_SkillEnd,
		FGameplayEventData()
	);
}

void APortalBreathActor::ApplyDamage()
{
	TArray<AActor*> OutOverlappingActors;
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
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			if (!TargetASC) continue;

			const bool bIsRolling = TargetASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Rolling);

			FGameplayEventData EventData;
			EventData.Instigator = CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor();
			EventData.Target = TargetASC->GetAvatarActor();

			if (bIsRolling)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
			}
			else
			{
				FDamageEffectParams Params = CachedDamageParams;
				Params.TargetAbilitySystemComponent = TargetASC;
				UHawnmunFunctionLibrary::ApplyDamageEffect(Params);
			}
		}
	}
}

void APortalBreathActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}