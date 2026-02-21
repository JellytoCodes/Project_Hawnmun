// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/ModMagCalc/MMC_MaxStamina.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	StrDef.AttributeToCapture = UHawnmunAttributeSet::GetStrengthAttribute();
	StrDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Str = 0.f;
	GetCapturedAttributeMagnitude(StrDef, Spec, EvaluationParameters, Str);
	Str = FMath::Max<float>(Str, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	return 50.f + 2.5f * Str + 15.f * PlayerLevel;
}