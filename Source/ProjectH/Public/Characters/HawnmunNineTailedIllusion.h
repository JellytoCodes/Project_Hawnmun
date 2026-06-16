// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunNineTailedIllusion.generated.h"

class UNiagaraComponent;
class UChaosClothAssetBase;
class UChaosClothComponent;

UCLASS()
class PROJECTH_API AHawnmunNineTailedIllusion : public ACharacter
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunNineTailedIllusion();

protected :
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Illusion Effects
public :
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActivateCharmIllusionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeactivateCharmIllusionEffect();

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Pattern|CharmIllusion")
	void SetCharmIllusionDissolveValue(float Value);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> CharmIllusionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|CharmIllusion")
	float CharmIllusionVisibleValue = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "NineTailed|Pattern|CharmIllusion")
	float CharmIllusionHiddenValue = 1.f;
#pragma endregion

#pragma region Combat And Damage
public:
	void CauseDamage();
	void AddIgnoreActor(AActor* InActor);

	FDamageEffectParams CombatDamageEffectParams;

	UFUNCTION(BlueprintImplementableEvent)
	void CauseDamageEffect();

private:
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
#pragma endregion

#pragma region Visual Phase
public :
	UFUNCTION(BlueprintCallable, Category = "NineTailed|Visual")
	void SetRobeForPhase(bool bPhase2);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Visual")
	void SetTailEarEffectsActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "NineTailed|Combat")
	void SetCharacterCollisionEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "NineTailed|Visual")
	bool IsPhase2VisualState() const;

	void ApplyPhase1VisualState();
	void ApplyPhase2VisualState();

private :
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

	bool bPhase2VisualState = false;
#pragma endregion
};