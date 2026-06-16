// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "HawnmunStructTypes.generated.h"

class UHawnmunGameplayAbility;
class UAbilitySystemComponent;
class UGameplayEffect;
class UNiagaraSystem;
class UMetaSoundSource;

#pragma region Combat And Damage
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};
#pragma endregion

#pragma region Animation And Movement
USTRUCT(BlueprintType)
struct FOrientationAngle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Forward;

	UPROPERTY(BlueprintReadWrite)
	float Right;

	UPROPERTY(BlueprintReadWrite)
	float Backward;

	UPROPERTY(BlueprintReadWrite)
	float Left;
};

USTRUCT(BlueprintType)
struct FTurnInPlaceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Yaw;

	UPROPERTY(BlueprintReadWrite)
	float YawLastTick;

	UPROPERTY(BlueprintReadWrite)
	float YawChangeOver;

	UPROPERTY(BlueprintReadWrite)
	float YawOffset;

	UPROPERTY(BlueprintReadWrite)
	float CurveValue;

	UPROPERTY(BlueprintReadWrite)
	float LastCurveValue;
};
#pragma endregion

#pragma region Ability System
USTRUCT(BlueprintType)
struct FHawnmunAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Abilities"))
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHawnmunGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString AbilityDescription = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
};
#pragma endregion

#pragma region Inventory And Items
USTRUCT(BlueprintType)
struct FItemBaseRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FConsumableRow : public FItemBaseRow
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ConsumableEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;
};

USTRUCT(BlueprintType)
struct FEquipmentRow : public FItemBaseRow
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EquipmentEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EquipSlotTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;
};
#pragma endregion

#pragma region Game Progression
USTRUCT(BlueprintType)
struct FDungeonInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DungeonTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DungeonName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText DungeonDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> TargetLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredLevel = 1;
};

USTRUCT(BlueprintType)
struct FLevelUpInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointAward = 1;
};
#pragma endregion

#pragma region FX Data 
USTRUCT(BlueprintType)
struct FHitReactRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "FX|Description")
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	TObjectPtr<UNiagaraSystem> HitVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	TObjectPtr<UMetaSoundSource> HitSound;
};
#pragma endregion