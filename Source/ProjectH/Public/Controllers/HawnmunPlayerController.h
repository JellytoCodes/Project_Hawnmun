// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Types/HawnmunEnumTypes.h"
#include "HawnmunPlayerController.generated.h"

class UHawnmunAbilitySystemComponent;
class UDataAsset_InputConfig;
class UAbilitySystemComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECTH_API AHawnmunPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	AHawnmunPlayerController();

	virtual FGenericTeamId GetGenericTeamId() const override;

	void EnableDefaultMappingContext() const;
	void DisableDefaultMappingContext() const;
	void SetInLobby(const bool InState);

	UFUNCTION(BlueprintCallable)
	void RestoreGameplayInput();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	FGenericTeamId PlayerId;
	bool bInLobby = false;

	UPROPERTY()
	APawn* ControlledPawn = nullptr;
#pragma endregion

#pragma region Input Setup And Bindings
public :
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetCinematicInputBlocked(bool bBlocked);

protected:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchTargetAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttributeWidgetAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryWidgetAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractableAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	bool bCinematicInputBlocked = false;
#pragma endregion

#pragma region Movement And Looking
private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void SwitchTarget(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region UI And Widget Management
public:
	void SetIsInteractable(const bool InIsInteractable, const EInteractableList InList, AActor* TargetActor);
	void DungeonMapWidgetSwitching();

	UFUNCTION(BlueprintCallable)
	void CloseCurrentWidget();

private:
	void AttributeWidgetSwitching();
	void InventoryWidgetSwitching();
	void InteractableByKey();
	void RequestCancel();
	void SetUIModeEnabled(bool bEnable);

	bool bIsInteractable = false;
	EInteractableList InteractableList = EInteractableList::None;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentInteractableTarget;
#pragma endregion

#pragma region Ability System Input
private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UHawnmunAbilitySystemComponent* GetASC();

	UPROPERTY()
	TWeakObjectPtr<UHawnmunAbilitySystemComponent> HawnmunAbilitySystemComponent;
#pragma endregion

#pragma region Screen Transition
public:
	UFUNCTION(BlueprintCallable, Category = "Screen Transition")
	void HoldBlackScreen();

	UFUNCTION(BlueprintCallable, Category = "Screen Transition")
	void FadeInFromBlack(float Duration = 1.f);

private:
	void StartInitialFadeIn();

	FTimerHandle InitialFadeInTimerHandle;
#pragma endregion
};