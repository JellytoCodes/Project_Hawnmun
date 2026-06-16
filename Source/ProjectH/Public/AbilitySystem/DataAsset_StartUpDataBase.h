// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h" // GameplayTag 사용을 위해 추가
#include "DataAsset_StartUpDataBase.generated.h"

class UHawnmunGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UInventoryComponent; // 인벤토리 컴포넌트 전방 선언

UENUM()
enum class EStartUpCharacterName : uint8
{
	Player,
	Pig,
	WildBoar,
	NineTailedFox
};

// [추가됨] 시작 시 지급할 아이템 정보를 담는 구조체
USTRUCT(BlueprintType)
struct FHawnmunStartUpItemInfo
{
	GENERATED_BODY()

	// 지급할 아이템의 고유 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;

	// 지급할 아이템의 수량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 ItemQuantity = 1;
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
	virtual void InitializeGameplayEffect(UAbilitySystemComponent* InASCToGive, EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel = 1);

	// [추가됨] 인벤토리 컴포넌트를 받아 시작 아이템을 지급하는 전용 메서드
	virtual void InitializeStartupItem(UInventoryComponent* InInventoryComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<FHawnmunStartUpAttributeInfo> StartUpAttributeInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities")
	TArray<TSubclassOf<UHawnmunGameplayAbility>> StartUpOffensiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities")
	TArray<TSubclassOf<UHawnmunGameplayAbility>> StartUpPassiveAbilities;

	// [추가됨] 에디터에서 설정할 시작 아이템 목록 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Items")
	TArray<FHawnmunStartUpItemInfo> StartUpItems;
};