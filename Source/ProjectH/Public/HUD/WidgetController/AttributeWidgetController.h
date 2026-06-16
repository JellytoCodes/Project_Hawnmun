// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/DataAsset_AttributeInfo.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "AttributeWidgetController.generated.h"

struct FGameplayAttribute;

#pragma region Data Structures And Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAttributeInfo&, Info);
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UAttributeWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()
	
#pragma region Core And Initialization
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
#pragma endregion

#pragma region Attribute Actions
public:
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void DowngradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void ConfirmUpgrades();

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void CancelUpgrades();

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void CloseAttributeWidgetByButton();
#pragma endregion

#pragma region Widget Delegates And Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Attributes", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_AttributeInfo> AttributeInfo;

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

	TMap<FGameplayTag, int32> PendingUpgrades;
#pragma endregion
};