// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class PROJECTH_API UHawnmunAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public :
	UHawnmunAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/////// Primary Attributes ///////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Vigor);
	//////////////////////////////////////////////////////////////////////

	/////// Secondary Attributes /////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, StaminaRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, MaxStamina);
	//////////////////////////////////////////////////////////////////////

	/////// Resistance Attributes ////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly,Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, PhysicalResistance);
	//////////////////////////////////////////////////////////////////////

	/////// Vital Attributes /////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, Stamina);
	//////////////////////////////////////////////////////////////////////

	/////// Meta Attributes //////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UHawnmunAttributeSet, IncomingXP);
	//////////////////////////////////////////////////////////////////////

private :
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void HandleStamina(const FEffectProperties& Props);

	void Debuff(const FEffectProperties& Props);
	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Props) const;
};
