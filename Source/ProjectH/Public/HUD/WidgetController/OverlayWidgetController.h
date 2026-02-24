// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttribueChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UOverlayWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

public :
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnMaxStaminaChanged;
};