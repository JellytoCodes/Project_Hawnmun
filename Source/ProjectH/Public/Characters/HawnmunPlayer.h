// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "HawnmunPlayer.generated.h"

class UInventoryComponent;
class UEquipmentComponent;
class AHawnmunPlayerController;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTH_API AHawnmunPlayer : public AHawnmunCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunPlayer();

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	void LoadProgress();
#pragma endregion

#pragma region Player State And Actions
public:
	UFUNCTION(BlueprintPure)
	bool GetIsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintSetter)
	void SetIsSprinting(const bool InSprinting) { bIsSprinting = InSprinting; }

	UFUNCTION(BlueprintCallable)
	void SetWeaponVisibility(bool bState);

	UFUNCTION(BlueprintCallable)
	void SetInLobby(const bool InState);

	void ProcessLooting(AActor* TargetActor);

private:
	UFUNCTION()
	void OnWeaponStateChanged(const bool bIsMainWeapon);
#pragma endregion

#pragma region Components And Data
public:
	UInventoryComponent* GetInventoryComponent();
	UEquipmentComponent* GetEquipmentComponent();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SubWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	TSubclassOf<UGameplayEffect> StaminaRegen;

	TWeakObjectPtr<AHawnmunPlayerController> PlayerController;
	bool bIsSprinting;
#pragma endregion

#pragma region Combat Interface
public:
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual FVector GetProjectileSpawnSocketLocation_Implementation(const FName SocketName) override;
	virtual void Die() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void DieEvent();
#pragma endregion

#pragma region Player Interface
public:
	virtual void EnableMappingContext_Implementation() override;
	virtual void DisableMappingContext_Implementation() override;
	virtual AHawnmunPlayerState* GetHawnmunPS_Implementation() override;
	virtual void SetIsInteractable_Implementation(const bool InIsInteractable, const EInteractableList InList, AActor* TargetActor = nullptr) override;

	virtual void ShowDungeonMap_Implementation() const override;
	virtual void HideDungeonMap_Implementation() const override;
#pragma endregion
};