// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "HawnmunDungeonSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSessionLootUpdatedSignature);

UCLASS()
class PROJECTH_API UHawnmunDungeonSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ==========================================
	// [Session Data]
	// ==========================================
	UFUNCTION(BlueprintCallable, Category = "SessionData|Session")
	void StartDungeonSession(FGameplayTag InDungeonTag);

	UFUNCTION(BlueprintCallable, Category = "SessionData|Session")
	void EndDungeonSession();

	UFUNCTION(BlueprintPure, Category = "SessionData|Session")
	bool IsInDungeonSession() const { return bIsDungeonSession; }

	// ==========================================
	// [Loot Bag]
	// ==========================================
	UFUNCTION(BlueprintCallable, Category = "SessionData|Loot")
	void RecordLootItem(const FGameplayTag& ItemTag, int32 Amount = 1);

	UFUNCTION(BlueprintPure, Category = "SessionData|Loot")
	const TMap<FGameplayTag, int32>& GetSessionLootBag() const { return SessionLootBag; }

	FOnSessionLootUpdatedSignature OnSessionLootUpdated;

	// ==========================================
	// [Checkpoint]
	// ==========================================
	UPROPERTY(BlueprintReadWrite, Category = "SessionData|Checkpoint")
	FTransform CurrentCheckpoint;

	UPROPERTY(BlueprintReadWrite, Category = "SessionData|Checkpoint")
	bool bHasCheckpoint = false;

	UFUNCTION(BlueprintCallable, Category = "SessionData|Checkpoint")
	void SaveCheckpoint(const FTransform& InTransform);

	UFUNCTION(BlueprintCallable, Category = "SessionData|Session")
	void MarkDungeonCleared();

	UFUNCTION(BlueprintPure, Category = "SessionData|Session")
	float GetClearTimeSeconds() const { return ClearTimeSeconds; }

	UFUNCTION(BlueprintPure, Category = "SessionData|Session")
	FGameplayTag GetCurrentDungeonTag() const { return CurrentDungeonTag; }

private:
	void ClearSessionData();

	bool bIsDungeonSession = false;

	UPROPERTY()
	TMap<FGameplayTag, int32> SessionLootBag;

	FGameplayTag CurrentDungeonTag;
	float DungeonStartTimeSeconds = 0.f;
	float ClearTimeSeconds = 0.f;
	bool bDungeonCleared = false;
};