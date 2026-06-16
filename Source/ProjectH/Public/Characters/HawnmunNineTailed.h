// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunEnemy.h"
#include "HawnmunNineTailed.generated.h"

class APortalBiteActor;
class APortalBreathActor;
class AFoxFireDropActor;
class AHawnmunNineTailedIllusion;
class UBehaviorTree;
class UChaosClothAssetBase;
class UChaosClothComponent;
class UNiagaraComponent;
class UDataLayerAsset;

UCLASS()
class PROJECTH_API AHawnmunNineTailed : public AHawnmunEnemy
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunNineTailed();

protected:
	virtual void BeginPlay() override;

private:
	void ValidateRequiredAssets() const;

	FVector CachedCenterLocation;
#pragma endregion

#pragma region Combat Interface And Phase
public:
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Die() override;
	virtual bool TryPhaseTransition() override;
	virtual void ResumeEnemyAI_Implementation() override;

	virtual void ResetToSpawnPoint() override;

	virtual ECombatDamageScalingStat GetDamageScalingStat_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Combat|Phase")
	void EnterPhase2();

private:
	void StartPhase2Transition();
	void StopNineTailedCombatLogic(const FString& Reason);
	void ClearNineTailedPatternTimers();
	void RefillPhase1Health();
	void RefillPhase2Health();
	void ClearPhase2State();

	void HandleDungeonCleared();

	void ForceCancelPlayerTargetLock();

	void MovePlayerToPhase2TransitionPoint() const;

	UPROPERTY(EditInstanceOnly, Category = "NineTailed|DungeonPhase")
	TObjectPtr<AActor> Phase2PlayerTransitionPoint;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Combat|Phase")
	float Phase1Health = 700.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Combat|Phase")
	float Phase2Health = 1300.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|AI")
	TObjectPtr<UBehaviorTree> Phase2BehaviorTree;
#pragma endregion

#pragma region Pattern Fox Fire
public:
	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|FoxFire")
	void ActiveFoxFireCardinalDrop(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|FoxFire")
	void ActiveFoxFireDiagonalDrop(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|FoxFire")
	void ActiveFoxFireRandDrop(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|FoxFire")
	void ActiveRandomSingleFoxFirePattern(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|FoxFire")
	void ActiveRandomSequenceFoxFirePattern(const FDamageEffectParams& DamageEffectParams);

private:
	void CreateFoxFireDrops();
	void ActiveRandomSequenceFoxFirePattern_Internal(bool bIsCardinalFirst, FDamageEffectParams DamageEffectParams);
	void ExecuteRandDrop(AFoxFireDropActor* DropActor, const FVector SpawnLocation, const FDamageEffectParams DamageParams);

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|FoxFire")
	TSoftClassPtr<AFoxFireDropActor> SpawnFoxFireDropClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AFoxFireDropActor>> FoxFireDrops;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|FoxFire")
	float FoxFireDropDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|FoxFire")
	float FoxFireRandDropRadius = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|FoxFire")
	float FoxFireRandDropInterval = 0.2f;

	UPROPERTY()
	TArray<FTimerHandle> FoxFireRandDropTimerHandles;
#pragma endregion

#pragma region Pattern Portal Breath
public:
	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|PortalBreath")
	void ActivePortalBreath(int32 Amount, const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|PortalBreath")
	void CancelPortalBreathPatterns();

private:
	void CreatePortalBreath();

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|PortalBreath")
	TSoftClassPtr<APortalBreathActor> SpawnPortalBreathClass;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|PortalBreath")
	float PortalSpawnDistance = 1500.f;

	UPROPERTY(Transient)
	TArray<TObjectPtr<APortalBreathActor>> FieldPortalBreath;
#pragma endregion

#pragma region Pattern Portal Bite
public:
	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|PortalBite")
	void ActivePortalBitePattern(const FVector& TargetLocation, const FDamageEffectParams& DamageParams, int32 Amount = 1, float Interval = 0.5f);

private:
	void CreatePortalBites();
	void ExecutePortalBite(FVector TargetLocation, FDamageEffectParams DamageParams);

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|PortalBite")
	TSoftClassPtr<APortalBiteActor> SpawnPortalBiteClass;

	UPROPERTY()
	TArray<TObjectPtr<APortalBiteActor>> PortalBitePool;

	int32 CurrentPortalBiteIndex = 0;

	UPROPERTY()
	TArray<FTimerHandle> PortalBiteTimerHandles;
#pragma endregion

#pragma region Pattern Charm Illusion
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "NineTailed|Pattern|CharmIllusion")
	void ActivateCharmIllusionEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "NineTailed|Pattern|CharmIllusion")
	void DeactivateCharmIllusionEffect();

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void ActivateIllusionsEffect();

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void DeactivateIllusionsEffect();

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void ShuffleNineTailedCharmIllusion();

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void BackCenterLocation(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void ExecuteIllusionSequence(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void SetCharmIllusionDissolveValue(float Value);

private:
	void CreateNineTailedIllusion();
	void GenerateCirclePoints(float Radius);

	void IllusionSequence_DisappearBody();
	void IllusionSequence_AppearWithIllusions();
	void IllusionSequence_DisappearAllAndDamage();
	void IllusionSequence_ReturnBody();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Pattern|CharmIllusion", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> CharmIllusionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|CharmIllusion")
	TSoftClassPtr<AHawnmunNineTailedIllusion> SpawnNineTailedIllusionClass;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|CharmIllusion")
	float CharmIllusionVisibleValue = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|CharmIllusion")
	float CharmIllusionHiddenValue = 1.f;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AHawnmunNineTailedIllusion>> NineTailedIllusion;

	UPROPERTY()
	TArray<FVector> NineTailedIllusionLocation;

	UPROPERTY(Transient)
	FDamageEffectParams CachedIllusionDamageParams;

	FTimerHandle IllusionSequenceTimerHandle;
#pragma endregion

#pragma region Visual Phase
public:
	UFUNCTION(BlueprintCallable, Category = "NineTailed|Visual")
	void SetRobeForPhase(bool bPhase2);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Visual")
	void SetTailEarEffectsActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Visual")
	void SetCharacterCollisionEnabled(bool bVisible);

	UFUNCTION(BlueprintPure, Category = "NineTailed|Visual")
	bool IsPhase2Active() const;

	void ApplyPhase1VisualState();
	void ApplyPhase2VisualState();

	void ForceEndIllusionPatternForPhaseTransition();
	void HideIllusionsForDeath();

private:
	void SetDungeonPhaseDataLayerState(bool bPhase2);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Cloth", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosClothComponent> BottomInnerClothes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Cloth", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosClothComponent> BottomOuterClothes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Cloth", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosClothComponent> RobeClothes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Cloth", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosClothComponent> TopClothes;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Visual|Cloth")
	TObjectPtr<UChaosClothAssetBase> Phase1RobeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Visual|Cloth")
	TObjectPtr<UChaosClothAssetBase> Phase2RobeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> EarEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect03;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect04;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect05;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NineTailed|Visual|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TailEffect06;

	UPROPERTY(EditInstanceOnly, Category = "NineTailed|DungeonPhase")
	TObjectPtr<UDataLayerAsset> Phase1DataLayer;

	UPROPERTY(EditInstanceOnly, Category = "NineTailed|DungeonPhase")
	TObjectPtr<UDataLayerAsset> Phase2DataLayer;
#pragma endregion
};
