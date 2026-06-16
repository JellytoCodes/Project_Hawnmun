// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "HawnmunGameModeBase.generated.h"

class AHawnmunEnemy;
class UDataAsset_AbilityInfo;
class UHawnmunSaveGame;
class USaveGame;
class UAudioComponent;

UCLASS()
class PROJECTH_API AHawnmunGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;
#pragma endregion

#pragma region Level Travel
public:
	UFUNCTION(BlueprintCallable, Category = "Travel")
	void TravelToMap(const FString& MapName);

	UFUNCTION(BlueprintCallable, Category = "Travel")
	void RetryForTravelToMap();
#pragma endregion

#pragma region Save And Load
public:
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SyncSlotToProgress(const FString& SlotName, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SyncProgressToSlot(const FString& SlotName, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveProgressData();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	UHawnmunSaveGame* GetProgressSaveData() const;

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	UHawnmunSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	void PopulateSaveGameObject(UHawnmunSaveGame* SaveGameObject, const FString& MapName);

	UPROPERTY(EditDefaultsOnly, Category = "GameMode Settings")
	TSubclassOf<USaveGame> SaveGameClass;

private:
	const FString ProgressSlotName = TEXT("AutoSave_Progress");
	const int32 ProgressSlotIndex = 99;
	const int32 MaxValidUserSlotIndex = 4;
#pragma endregion

#pragma region Audio Management
public:
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayBGM(USoundBase* NewSound, const float FadeInTime = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ChangeBGM(USoundBase* NewSound, const float FadeInTime = 0.f, const float FadeOutTime = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopBGM(const float FadeOutTime = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayBGMByTag(FGameplayTag BGMTag, const float FadeInTime = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ChangeBGMByTag(FGameplayTag BGMTag, const float FadeInTime = 0.f, const float FadeOutTime = 0.f);

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|BGM")
	TMap<FGameplayTag, USoundBase*> BGMMap;
#pragma endregion

#pragma region Combat And Encounters
public:
	UFUNCTION(BlueprintCallable, Category = "Combat|Encounter")
	void AddAggroEnemy(AHawnmunEnemy* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Combat|Encounter")
	void RemoveAggroEnemy(AHawnmunEnemy* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Combat|Encounter")
	void ResetAllAggroEnemies();

	UFUNCTION(BlueprintCallable, Category = "Combat|Encounter")
	void RequestRespawn(AController* InController);

private:
	bool IsBossEnemy(const AHawnmunEnemy* Enemy) const;

	UPROPERTY()
	TSet<TObjectPtr<AHawnmunEnemy>> ActiveEnemies;
#pragma endregion

#pragma region Dungeon System
public:
	UFUNCTION(BlueprintCallable, Category = "Game|Dungeon")
	void MergeDungeonLoot(AController* InController);

	UFUNCTION(BlueprintCallable, Category = "Game|Dungeon")
	void RegisterDungeonCleared();

	UFUNCTION(BlueprintCallable, Category = "Game|Dungeon")
	void CompleteDungeonClear(AController* InController);

	UFUNCTION(BlueprintCallable, Category = "Game|Dungeon")
	void AbandonDungeon();

	UFUNCTION(BlueprintPure, Category = "Game|Dungeon")
	float GetDungeonClearTimeSeconds() const;

	UFUNCTION(BlueprintPure, Category = "Game|Dungeon")
	FText GetDungeonClearTimeText() const;
#pragma endregion
};