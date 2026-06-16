// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailedIllusion.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HawnmunFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ChaosClothAsset/ClothComponent.h"
#include "Interfaces/PlayerInterface.h"

#pragma region Core And Initialization
AHawnmunNineTailedIllusion::AHawnmunNineTailedIllusion()
{
	PrimaryActorTick.bCanEverTick = false;

	CharmIllusionEffect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	CharmIllusionEffect->SetAutoActivate(false);
	CharmIllusionEffect->SetupAttachment(GetMesh());

	BottomInnerClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_BottomInnerClothes"));
	BottomInnerClothes->SetupAttachment(GetMesh());

	BottomOuterClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_BottomOuterClothes"));
	BottomOuterClothes->SetupAttachment(GetMesh());

	RobeClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_Robe"));
	RobeClothes->SetupAttachment(GetMesh());

	TopClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_TopClothes"));
	TopClothes->SetupAttachment(GetMesh());

	EarEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EarEffect"));
	EarEffect->SetupAttachment(GetMesh(), TEXT("EarSocket"));
	EarEffect->SetAutoActivate(false);

	TailEffect01 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_01"));
	TailEffect01->SetupAttachment(GetMesh(), TEXT("Tail_01"));
	TailEffect01->SetAutoActivate(false);

	TailEffect02 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_02"));
	TailEffect02->SetupAttachment(GetMesh(), TEXT("Tail_02"));
	TailEffect02->SetAutoActivate(false);

	TailEffect03 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_03"));
	TailEffect03->SetupAttachment(GetMesh(), TEXT("Tail_03"));
	TailEffect03->SetAutoActivate(false);

	TailEffect04 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_04"));
	TailEffect04->SetupAttachment(GetMesh(), TEXT("Tail_04"));
	TailEffect04->SetAutoActivate(false);

	TailEffect05 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_05"));
	TailEffect05->SetupAttachment(GetMesh(), TEXT("Tail_05"));
	TailEffect05->SetAutoActivate(false);

	TailEffect06 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_06"));
	TailEffect06->SetupAttachment(GetMesh(), TEXT("Tail_06"));
	TailEffect06->SetAutoActivate(false);
}

void AHawnmunNineTailedIllusion::BeginPlay()
{
	Super::BeginPlay();

	checkf(Phase1RobeAsset, TEXT("ApplyPhase1VisualState failed: Phase1RobeAsset is not assigned."));
	checkf(Phase2RobeAsset, TEXT("ApplyPhase2VisualState failed: Phase2RobeAsset is not assigned."));

	ApplyPhase1VisualState();
}

void AHawnmunNineTailedIllusion::SetCharmIllusionDissolveValue(float Value)
{
	const FName NiagaraAnimationName = TEXT("User_Animation");
	const FName MaterialAnimationName = TEXT("Animation");

	if (CharmIllusionEffect)
	{
		CharmIllusionEffect->SetVariableFloat(NiagaraAnimationName, Value);
	}

	if (GetMesh())
	{
		GetMesh()->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (BottomInnerClothes)
	{
		BottomInnerClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (BottomOuterClothes)
	{
		BottomOuterClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (RobeClothes)
	{
		RobeClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (TopClothes)
	{
		TopClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}
}
#pragma endregion

#pragma region Combat And Damage
void AHawnmunNineTailedIllusion::CauseDamage()
{
	CauseDamageEffect();
	TArray<AActor*> OverlappingActors;

	UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(
		this,
		OverlappingActors,
		ActorsToIgnore,
		300.f,
		GetActorLocation());

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->Implements<UPlayerInterface>())
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			if (TargetASC)
			{
				FDamageEffectParams Params = CombatDamageEffectParams;
				Params.TargetAbilitySystemComponent = TargetASC;
				UHawnmunFunctionLibrary::ApplyDamageEffect(Params);
			}
		}
	}
}

void AHawnmunNineTailedIllusion::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}

void AHawnmunNineTailedIllusion::SetRobeForPhase(bool bPhase2)
{
	bPhase2VisualState = bPhase2;

	RobeClothes->SetVisibility(true, true);
	RobeClothes->SetAsset(bPhase2 ? Phase2RobeAsset : Phase1RobeAsset);
	RobeClothes->ForceNextUpdateTeleportAndReset();
}

void AHawnmunNineTailedIllusion::SetTailEarEffectsActive(bool bActive)
{
	if (bActive && !IsPhase2VisualState()) return;

	static const FName ColorParameterName(TEXT("User.Opacity"));
	constexpr float ActiveOpacityValue = 1.f;

	UNiagaraComponent* Effects[] =
	{
		EarEffect,
		TailEffect01,
		TailEffect02,
		TailEffect03,
		TailEffect04,
		TailEffect05,
		TailEffect06
	};

	for (UNiagaraComponent* Effect : Effects)
	{
		if (!Effect) continue;

		Effect->SetVariableFloat(ColorParameterName,bActive ? ActiveOpacityValue : 0.f);
		bActive ? Effect->Activate(true) : Effect->Deactivate();
	}
}

void AHawnmunNineTailedIllusion::SetCharacterCollisionEnabled(bool bVisible)
{
	SetActorEnableCollision(bVisible);
}

bool AHawnmunNineTailedIllusion::IsPhase2VisualState() const
{
	return bPhase2VisualState;
}

void AHawnmunNineTailedIllusion::ApplyPhase1VisualState()
{
	SetRobeForPhase(false);
	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(false);
	SetCharmIllusionDissolveValue(CharmIllusionHiddenValue);
}

void AHawnmunNineTailedIllusion::ApplyPhase2VisualState()
{
	SetRobeForPhase(true);
	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(false);
	SetCharmIllusionDissolveValue(CharmIllusionHiddenValue);
}
#pragma endregion
