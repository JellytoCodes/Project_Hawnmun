// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HawnmunHUD.generated.h"

class USettingsWidgetController;
class UMediaPlayer;
class UMediaSource;
class UMediaSoundComponent;
class UCinematicWidgetController;
class UDungeonMapWidgetController;
class UInventoryWidgetController;
class UAttributeWidgetController;
class USaveSlotWidgetController; 
class UHawnmunUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;

UCLASS()
class PROJECTH_API AHawnmunHUD : public AHUD
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	void InitializeWidgets(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);

private:
	FVector2D ViewportSize;
	bool bWidgetsInitialized = false;
#pragma endregion

#pragma region Overlay Widget
public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void ShowOverlay() const;

	UFUNCTION(BlueprintCallable)
	void HideOverlay() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Overlay")
	TSubclassOf<UHawnmunUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
#pragma endregion

#pragma region Attribute Widget
public:
	UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& WCParams);
	void ShowAttribute() const;
	void HideAttribute() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> AttributeWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Attribute")
	TSubclassOf<UHawnmunUserWidget> AttributeWidgetClass;

	UPROPERTY()
	TObjectPtr<UAttributeWidgetController> AttributeWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Attribute")
	TSubclassOf<UAttributeWidgetController> AttributeWidgetControllerClass;
#pragma endregion

#pragma region Inventory Widget
public:
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	void ShowInventory() const;
	void HideInventory() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Inventory")
	TSubclassOf<UHawnmunUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> PlayerInfoWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Inventory")
	TSubclassOf<UHawnmunUserWidget> PlayerInfoWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Inventory")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
#pragma endregion

#pragma region Dungeon Map Widget
public:
	UDungeonMapWidgetController* GetDungeonWidgetController(const FWidgetControllerParams& WCParams);
	void ShowDungeonMap() const;
	void HideDungeonMap() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> DungeonWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|DungeonMap")
	TSubclassOf<UHawnmunUserWidget> DungeonWidgetClass;

	UPROPERTY()
	TObjectPtr<UDungeonMapWidgetController> DungeonWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|DungeonMap")
	TSubclassOf<UDungeonMapWidgetController> DungeonWidgetControllerClass;
#pragma endregion	

#pragma region Save Slot Widget
public:
	USaveSlotWidgetController* GetSaveSlotWidgetController(const FWidgetControllerParams& WCParams);
	void ShowSaveSlot() const;
	void HideSaveSlot() const;

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

#pragma region Cinematic Widget
public:
	UFUNCTION(BlueprintPure)
	UCinematicWidgetController* GetCinematicSlotWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void ShowCinematicWidget() const;

	UFUNCTION(BlueprintCallable)
	void HideCinematicWidget() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> CinematicWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|CinematicWidget")
	TSubclassOf<UHawnmunUserWidget> CinematicWidgetClass;

	UPROPERTY()
	TObjectPtr<UCinematicWidgetController> CinematicWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|CinematicWidget")
	TSubclassOf<UCinematicWidgetController> CinematicWidgetControllerClass;
#pragma endregion

#pragma region Mission Clear Widget
public:
	void ShowMissionClearWidget() const;
	void HideMissionClearWidget() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> MissionClearWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Mission")
	TSubclassOf<UHawnmunUserWidget> MissionClearWidgetClass;
#pragma endregion

#pragma region Pause Menu Widget
public:
	void ShowPauseMenu() const;
	void HidePauseMenu() const;

private:
	UPROPERTY()
	TObjectPtr<UHawnmunUserWidget> PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass|Pause")
	TSubclassOf<UHawnmunUserWidget> PauseMenuWidgetClass;
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