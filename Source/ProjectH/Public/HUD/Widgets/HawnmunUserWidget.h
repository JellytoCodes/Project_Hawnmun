// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HawnmunUserWidget.generated.h"

UCLASS()
class PROJECTH_API UHawnmunUserWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Widget Controller
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
#pragma endregion

#pragma region Widget Animation
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Animation")
	void PlayOpenAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Animation")
	void PlayCloseAnimation();
#pragma endregion
};