// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/HawnmunTitleHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/Widgets/HawnmunUserWidget.h"
#include "HUD/WidgetController/SaveSlotWidgetController.h"
#include "HUD/WidgetController/SettingsWidgetController.h"
#include "Engine/Engine.h"

#pragma region Core And Initialization
void AHawnmunTitleHUD::InitializeTitleWidgets(APlayerController* PC)
{
	FWidgetControllerParams ValidParams(PC, nullptr, nullptr, nullptr);

	if (SaveSlotWidgetClass != nullptr && SaveSlotWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SaveSlotWidgetClass);
		SaveSlotWidget = Cast<UHawnmunUserWidget>(Widget);

		USaveSlotWidgetController* WidgetController = GetSaveSlotWidgetController(ValidParams);
		WidgetController->SetIsTitleMenu(true);
		SaveSlotWidget->SetWidgetController(WidgetController);
		
		SaveSlotWidget->AddToViewport();

		FVector2D CurrentViewportSize;
		GEngine->GameViewport->GetViewportSize(CurrentViewportSize);
		
		SaveSlotWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		SaveSlotWidget->SetPositionInViewport(FVector2D(CurrentViewportSize.X - 50.f, CurrentViewportSize.Y * 0.5f), true);

		SaveSlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SettingsWidgetClass != nullptr && SettingsWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SettingsWidgetClass);
		SettingsWidget = Cast<UHawnmunUserWidget>(Widget);

		USettingsWidgetController* WidgetController = GetSettingsWidgetController(ValidParams);
		
		SettingsWidget->SetWidgetController(WidgetController);
		
		SettingsWidget->AddToViewport();

		FVector2D CurrentViewportSize;
		GEngine->GameViewport->GetViewportSize(CurrentViewportSize);
		
		SettingsWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		SettingsWidget->SetPositionInViewport(FVector2D(CurrentViewportSize.X - 50.f, CurrentViewportSize.Y * 0.5f), true);

		SettingsWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
#pragma endregion

#pragma region Save Slot Widget
USaveSlotWidgetController* AHawnmunTitleHUD::GetSaveSlotWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SaveSlotWidgetController == nullptr)
	{
		SaveSlotWidgetController = NewObject<USaveSlotWidgetController>(this, SaveSlotWidgetControllerClass);
		SaveSlotWidgetController->SetWidgetControllerParams(WCParams);
	}
	return SaveSlotWidgetController;
}

void AHawnmunTitleHUD::ShowSaveSlot()
{
	SaveSlotWidget->SetVisibility(ESlateVisibility::Visible);
	SaveSlotWidget->PlayOpenAnimation();
}

void AHawnmunTitleHUD::HideSaveSlot()
{
	SaveSlotWidget->PlayCloseAnimation();
}
#pragma endregion

#pragma region Settings Widget
USettingsWidgetController* AHawnmunTitleHUD::GetSettingsWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SettingsWidgetController == nullptr)
	{
		SettingsWidgetController = NewObject<USettingsWidgetController>(this, SettingsWidgetControllerClass);
		SettingsWidgetController->SetWidgetControllerParams(WCParams);
	}
	return SettingsWidgetController;
}

void AHawnmunTitleHUD::ShowSettingsWidget()
{
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
	SettingsWidget->PlayOpenAnimation();
}

void AHawnmunTitleHUD::HideSettingsWidget()
{

}
#pragma endregion