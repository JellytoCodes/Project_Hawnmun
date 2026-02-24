// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HawnmunHUD.generated.h"

class UHawnmunUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;

UCLASS()
class PROJECTH_API AHawnmunHUD : public AHUD
{
	GENERATED_BODY()

public :
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void ShowOverlay();
	void HideOverlay();

private :
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Overlay")
	TSubclassOf<UHawnmunUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
};
