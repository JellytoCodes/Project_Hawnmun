// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/HawnmunHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/Widgets/HawnmunUserWidget.h"
#include "HUD/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AHawnmunHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AHawnmunHUD::InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UHawnmunUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}

void AHawnmunHUD::ShowOverlay()
{
	OverlayWidget->SetVisibility(ESlateVisibility::Visible);
}

void AHawnmunHUD::HideOverlay()
{
	OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
}
