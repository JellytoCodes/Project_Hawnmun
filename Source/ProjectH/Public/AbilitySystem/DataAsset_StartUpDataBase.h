// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UHawnmunGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;

UENUM()
enum class EStartUpCharacterName : uint8
{
	Player,
	Pig,
	WildBoar,
	NineTailedFox
};

USTRUCT(BlueprintType)
struct FHawnmunStartUpAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PrimaryGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SecondaryGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> VitalGameplayEffect;
};

UCLASS()
class PROJECTH_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public :
	virtual void InitializeGameplayEffect(UAbilitySystemComponent* InASCToGive,EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel = 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<FHawnmunStartUpAttributeInfo> StartUpAttributeInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities")
	TArray<TSubclassOf<UHawnmunGameplayAbility>> StartUpOffensiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities")
	TArray<TSubclassOf<UHawnmunGameplayAbility>> StartUpPassiveAbilities;
};
