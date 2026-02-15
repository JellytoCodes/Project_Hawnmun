// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunGameplayAbility.generated.h"

class AHawnmunEnemy;
class AHawnmunPlayer;
class UHawnmunAbilitySystemComponent;

UCLASS()
class PROJECTH_API UHawnmunGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected :
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	UHawnmunAbilitySystemComponent* GetFatedBrandAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AHawnmunPlayer* GetFatedBrandCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AHawnmunEnemy* GetFatedBrandEnemyFromActorInfo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
	float CooldownTime = 0.f;


private :
	TWeakObjectPtr<AHawnmunPlayer> CachedFatedBrandCharacter;
	TWeakObjectPtr<AHawnmunEnemy> CachedFatedBrandEnemy;
};
