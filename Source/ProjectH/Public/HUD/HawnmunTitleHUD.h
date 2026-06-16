// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HawnmunTitleHUD.generated.h"

class USettingsWidgetController;
class USaveSlotWidgetController;
class UHawnmunUserWidget;
class APlayerController;
struct FWidgetControllerParams;

UCLASS()
class PROJECTH_API AHawnmunTitleHUD : public AHUD
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	UFUNCTION(BlueprintCallable, Category = "UI|Init")
	void InitializeTitleWidgets(APlayerController* PC);
#pragma endregion

#pragma region Save Slot Widget
public:
	USaveSlotWidgetController* GetSaveSlotWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowSaveSlot();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideSaveSlot();

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> SaveSlotWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|SaveSlot")
	TSubclassOf<UHawnmunUserWidget> SaveSlotWidgetClass;

	UPROPERTY()
	TObjectPtr<USaveSlotWidgetController> SaveSlotWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|SaveSlot")
	TSubclassOf<USaveSlotWidgetController> SaveSlotWidgetControllerClass;
#pragma endregion

#pragma region Settings Widget
public:
	USettingsWidgetController* GetSettingsWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowSettingsWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideSettingsWidget();

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> SettingsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|SettingsWidget")
	TSubclassOf<UHawnmunUserWidget> SettingsWidgetClass;

	UPROPERTY()
	TObjectPtr<USettingsWidgetController> SettingsWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|SettingsWidget")
	TSubclassOf<USettingsWidgetController> SettingsWidgetControllerClass;
#pragma endregion
};