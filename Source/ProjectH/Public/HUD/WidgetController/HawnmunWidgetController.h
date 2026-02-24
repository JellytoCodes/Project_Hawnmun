// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HawnmunWidgetController.generated.h"

class UHawnmunAttributeSet;
class UHawnmunAbilitySystemComponent;
class AHawnmunPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class PROJECTH_API UHawnmunWidgetController : public UObject
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() { }
	virtual void BindCallbacksToDependencies() { }

protected :
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<AHawnmunPlayerController> HawnmunPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UHawnmunAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UHawnmunAttributeSet> HawnmunAttributeSet;

	AHawnmunPlayerController* GetHawnmunPC();
	UHawnmunAbilitySystemComponent* GetHawnmunASC();
	UHawnmunAttributeSet* GetHawnmunAS();
};
