// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "HawnmunPlayerState.generated.h"

class UHawnmunSaveGame;
class UEquipmentComponent;
class UInventoryComponent;
class UAttributeSet;

#pragma region Data Structures And Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* Stat Level */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /* Stat Level */, bool /* Level Up */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChanged, const bool);
#pragma endregion


UCLASS()
class PROJECTH_API AHawnmunPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region Core And Components
public:
	AHawnmunPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;
#pragma endregion

#pragma region Player Progression
public:
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnLevelChanged OnLevelChangeDelegate;

	void AddToXP(int32 InXP);
	void AddToAttributePoints(int32 InPoints);

	int32 GetPlayerLevel() const { return Level; }
	int32 GetPlayerXP() const { return XP; }
	int32 GetAttributePoints() const { return AttributePoints; }
	int32 GetNextLevelRequirement() const;

	void SetPlayerLevel(int32 InLevel) { Level = InLevel; }
	void SetXP(int32 InXP) { XP = InXP; }
	void SetAttributePoints(int32 InPoints) { AttributePoints = InPoints; } // Save And Load
	int32 GetCurrentLevelRequirement() const;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Level Data")
	UDataTable* LevelUpDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 AttributePoints = 0;
#pragma endregion

#pragma region Inventory And Equipment
public:
	FOnEquippedWeaponChanged OnEquippedWeaponChangedDelegate;

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

	UFUNCTION(Category = "Inventory|Action")
	bool UseConsumableItem(FGameplayTag ItemTag);

	UFUNCTION(Category = "Inventory|Action")
	void EquipItem(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Action")
	bool UseEquippedPotion();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Action")
	bool UseEquippedAmmo();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquippedWeaponChanged(const bool bInFlag);

	UFUNCTION(BlueprintCallable, Category = "SessionData|Loot")
	void CommitDungeonLoot();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	bool bIsEquippedMainWeapon = true;
#pragma endregion

#pragma region Save And Load
public:
	void SaveInventoryAndEquipment(UHawnmunSaveGame* SaveGame);
	void LoadInventoryAndEquipment(UHawnmunSaveGame* SaveGame);
#pragma endregion
};