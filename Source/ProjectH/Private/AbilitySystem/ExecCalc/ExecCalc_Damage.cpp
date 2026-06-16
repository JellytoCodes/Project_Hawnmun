// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Interfaces/CombatInterface.h"

namespace HawnmunCombatConstants
{
	constexpr float ArmorMitigationConstant = 100.f;
	constexpr float BlockedDamageMultiplier = 0.5f;
	constexpr float BaseCriticalDamageMultiplier = 1.5f;

	// 주 능력치 1당 추가되는 기본 데미지
	constexpr float PrimaryStatDamageCoefficient = 1.f;
}

struct HawnmunDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	HawnmunDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, Intelligence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHawnmunAttributeSet, CriticalHitResistance, Target, false);
	}
};

static const HawnmunDamageStatics& DamageStatics()
{
	static HawnmunDamageStatics Statics;
	return Statics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Primary_Strength, DamageStatics().StrengthDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Primary_Intelligence, DamageStatics().IntelligenceDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	checkf(SourceASC, TEXT("SourceASC is null in ExecCalc_Damage"));
	checkf(TargetASC, TEXT("TargetASC is null in ExecCalc_Damage"));

	AActor* SourceAvatar = SourceASC->GetAvatarActor();
	AActor* TargetAvatar = TargetASC->GetAvatarActor();

	checkf(SourceAvatar, TEXT("SourceAvatar is null in ExecCalc_Damage"));
	checkf(TargetAvatar, TEXT("TargetAvatar is null in ExecCalc_Damage"));

	checkf(SourceAvatar->Implements<UCombatInterface>(), TEXT("Damage source does not implement CombatInterface: %s"), *GetNameSafe(SourceAvatar));

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);

	float Damage = 0.f;

	for (const FGameplayTag& DamageTypeTag : HawnmunGameplayTags::DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		if (DamageTypeValue <= 0.f)
		{
			continue;
		}

		Damage += DamageTypeValue;
	}

	// 공격자가 사용할 주 능력치를 CombatInterface로 결정한다.
	const ECombatDamageScalingStat ScalingStat = ICombatInterface::Execute_GetDamageScalingStat(SourceAvatar);

	float SourcePrimaryStat = 0.f;

	switch (ScalingStat)
	{
	case ECombatDamageScalingStat::Strength:
		SourcePrimaryStat = GetCapturedMagnitude(ExecutionParams, DamageStatics().StrengthDef, EvaluationParameters);
		break;
	case ECombatDamageScalingStat::Intelligence:
		SourcePrimaryStat = GetCapturedMagnitude(ExecutionParams, DamageStatics().IntelligenceDef, EvaluationParameters);
		break;
	default:
		checkNoEntry();
		break;
	}

	if (Damage > 0.f)
	{
		Damage += SourcePrimaryStat * HawnmunCombatConstants::PrimaryStatDamageCoefficient;
	}

	// 주 능력치까지 포함한 전체 데미지에 방사형 감쇠를 적용한다.
	if (UHawnmunFunctionLibrary::IsRadialDamage(EffectContextHandle))
	{
		const FVector Origin = UHawnmunFunctionLibrary::GetRadialDamageOrigin(EffectContextHandle);
		const float InnerRadius = UHawnmunFunctionLibrary::GetRadialDamageInnerRadius(EffectContextHandle);
		const float OuterRadius = UHawnmunFunctionLibrary::GetRadialDamageOuterRadius(EffectContextHandle);

		checkf(OuterRadius > InnerRadius, TEXT("Invalid radial damage radius: OuterRadius must be greater than InnerRadius. InnerRadius=%f, OuterRadius=%f"), InnerRadius, OuterRadius);

		const float Distance = FVector::Dist(Origin, TargetAvatar->GetActorLocation());
		const float FalloffRatio = 1.f - FMath::Clamp((Distance - InnerRadius) / (OuterRadius - InnerRadius), 0.f, 1.f);

		Damage *= FalloffRatio;
	}

	// 방어력 관통
	const float TargetArmor = GetCapturedMagnitude(ExecutionParams, DamageStatics().ArmorDef, EvaluationParameters);
	float SourceArmorPenetration = GetCapturedMagnitude(ExecutionParams, DamageStatics().ArmorPenetrationDef, EvaluationParameters);

	SourceArmorPenetration = FMath::Clamp(SourceArmorPenetration, 0.f, 100.f);
	const float EffectiveArmor = FMath::Max(TargetArmor * (1.f - SourceArmorPenetration / 100.f), 0.f);
	const float MitigationRate = EffectiveArmor / (HawnmunCombatConstants::ArmorMitigationConstant + EffectiveArmor);

	Damage *= 1.f - MitigationRate;

	// 방어
	const float TargetBlockChance = GetCapturedMagnitude(ExecutionParams, DamageStatics().BlockChanceDef, EvaluationParameters);

	const bool bBlocked = FMath::FRandRange(0.f, 100.f) < TargetBlockChance;

	UHawnmunFunctionLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	if (bBlocked)
	{
		Damage *= HawnmunCombatConstants::BlockedDamageMultiplier;
	}

	// 치명타
	const float SourceCriticalHitChance = GetCapturedMagnitude(ExecutionParams, DamageStatics().CriticalHitChanceDef, EvaluationParameters);
	const float SourceCriticalHitDamage = GetCapturedMagnitude(ExecutionParams, DamageStatics().CriticalHitDamageDef, EvaluationParameters);
	float TargetCriticalHitResistance = GetCapturedMagnitude(ExecutionParams, DamageStatics().CriticalHitResistanceDef, EvaluationParameters);

	TargetCriticalHitResistance = FMath::Clamp(TargetCriticalHitResistance, 0.f, 100.f);

	const float EffectiveCriticalHitChance = SourceCriticalHitChance * (1.f - TargetCriticalHitResistance / 100.f);

	const bool bCriticalHit = FMath::FRandRange(0.f, 100.f) < EffectiveCriticalHitChance;

	UHawnmunFunctionLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	if (bCriticalHit)
	{
		Damage *= HawnmunCombatConstants::BaseCriticalDamageMultiplier;
		Damage += SourceCriticalHitDamage;
	}

	int32 FinalDamage = FMath::RoundToInt(Damage);
	FinalDamage = FMath::Max(FinalDamage, 1);

	const FGameplayModifierEvaluatedData EvaluatedData(UHawnmunAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, static_cast<float>(FinalDamage));

	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

float UExecCalc_Damage::GetCapturedMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDefinition, const FAggregatorEvaluateParameters& EvaluateParameters) const
{
	float Value = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluateParameters, Value);

	return FMath::Max(Value, 0.f);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> InTagsDefs) const
{
	// 디버프 관련 기능 추가 계획 없음
}