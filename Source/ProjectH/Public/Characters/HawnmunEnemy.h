// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "Interfaces/EnemyInterface.h"
#include "HawnmunEnemy.generated.h"

class UWidgetComponent;
class AHawnmunAIController;
class UBehaviorTree;
class USphereComponent;

#pragma region Data Structures
USTRUCT(BlueprintType)
struct FLootData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DropChance = 100.0f;
};
#pragma endregion

UCLASS()
class PROJECTH_API AHawnmunEnemy : public AHawnmunCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunEnemy();

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHawnmunAIController> HawnmunAIController;

	FTransform InitialSpawnTransform;
#pragma endregion

#pragma region Combat And State Management
public:
	void SetSpawnedActorByBlackBoardKey(AActor* spawnedActor);
	void SetEnemyActive(bool bActive, AActor* InActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Combat|Encounter")
	virtual void ResetToSpawnPoint();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DieEvent();

	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Die() override;
	virtual bool TryPhaseTransition() override;

	virtual void SetAggroTarget_Implementation(AActor* TargetActor) override;

	bool IsEnemyActive() const { return bIsEnemyActive; }
	void SetStartInactiveForPooling() { bIsEnemyActive = false; }

protected :
	void HideBossOverlay() const;

private :
	bool bIsEnemyActive = true;
#pragma endregion

#pragma region Loot And Interaction
public:
	UFUNCTION(BlueprintCallable, Category = "Loot")
	TMap<FGameplayTag, int32> ExtractLoot();

protected:
	UFUNCTION()
	virtual void OnBeginDropItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndDropItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DropItem")
	TObjectPtr<USphereComponent> DorpItemInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DropItem")
	TObjectPtr<UWidgetComponent> DropItemWidget;

	UPROPERTY(EditAnywhere, Category = "Loot")
	TMap<FGameplayTag, FLootData> LootTable;

	bool bIsLooted = false;
#pragma endregion
};