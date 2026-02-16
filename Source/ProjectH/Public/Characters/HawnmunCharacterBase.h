// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DataAsset_StartUpDataBase.h"
#include "GameFramework/Character.h"
#include "HawnmunCharacterBase.generated.h"

class UHawnmunAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECTH_API AHawnmunCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHawnmunCharacterBase();

	UFUNCTION(BlueprintPure)
	UHawnmunAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> HawnmunAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterData")
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> StartUpData;

	virtual void InitAbilityActorInfo();

	void AddCharacterAbilities() const;
};
