// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HawnmunWidgetController.generated.h"

class AHawnmunPlayerState;
class UHawnmunAttributeSet;
class UHawnmunAbilitySystemComponent;
class AHawnmunPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

#pragma region Data Structures And Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS)
		: PlayerController(PC), AbilitySystemComponent(ASC), AttributeSet(AS), PlayerState(PS) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;
};
#pragma endregion

UCLASS()
class PROJECTH_API UHawnmunWidgetController : public UObject
{
	GENERATED_BODY()

#pragma region Initialization And Core Functions
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() { }
	
	virtual void BindCallbacksToDependencies() { }
#pragma endregion

#pragma region Base Widget Data
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<APlayerState> PlayerState;
#pragma endregion

#pragma region Custom Widget Data And Getters
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<AHawnmunPlayerController> HawnmunPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UHawnmunAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UHawnmunAttributeSet> HawnmunAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<AHawnmunPlayerState> HawnmunPlayerState;

	AHawnmunPlayerController* GetHawnmunPC();
	UHawnmunAbilitySystemComponent* GetHawnmunASC();
	UHawnmunAttributeSet* GetHawnmunAS();
	AHawnmunPlayerState* GetHawnmunPS();
#pragma endregion
};