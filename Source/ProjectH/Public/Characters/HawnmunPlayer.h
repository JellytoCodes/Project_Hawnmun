// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "HawnmunPlayer.generated.h"

class AHawnmunPlayerController;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTH_API AHawnmunPlayer : public AHawnmunCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public :
	AHawnmunPlayer();

	UFUNCTION(BlueprintGetter)
	bool GetIsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintSetter)
	void SetIsSprinting(const bool InSprinting) { bIsSprinting = InSprinting; }

#pragma region CombatInterface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Die() override;
#pragma endregion

#pragma region PlayerInterface
	virtual void EnableMappingContext_Implementation() override;
	virtual void DisableMappingContext_Implementation() override;
#pragma endregion
protected:
	virtual void InitAbilityActorInfo() override;

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

private :
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	TWeakObjectPtr<AHawnmunPlayerController> PlayerController;

	bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	TSubclassOf<UGameplayEffect> StaminaRegen;
};