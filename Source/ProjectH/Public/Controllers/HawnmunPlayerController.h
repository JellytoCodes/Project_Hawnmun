// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "HawnmunPlayerController.generated.h"

class UHawnmunAbilitySystemComponent;
class UDataAsset_InputConfig;
class UAbilitySystemComponent;
struct FInputActionValue;

class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECTH_API AHawnmunPlayerController : public APlayerController
{
	GENERATED_BODY()

public :
	AHawnmunPlayerController();

protected :
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UHawnmunAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	UPROPERTY()
	TWeakObjectPtr<UHawnmunAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY()
	APawn* ControlledPawn = nullptr;
};
