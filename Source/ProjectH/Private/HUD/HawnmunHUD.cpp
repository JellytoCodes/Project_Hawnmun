// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/HawnmunHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/Widgets/HawnmunUserWidget.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "HUD/WidgetController/AttributeWidgetController.h"
#include "HUD/WidgetController/CinematicWidgetController.h"
#include "HUD/WidgetController/DungeonMapWidgetController.h"
#include "HUD/WidgetController/InventoryWidgetController.h"
#include "HUD/WidgetController/SaveSlotWidgetController.h"
#include "HUD/WidgetController/SettingsWidgetController.h"

#pragma region Core And Initialization

void AHawnmunHUD::InitializeWidgets(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS)
{
	if (bWidgetsInitialized) return;

	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS, PS);

	if (GEngine)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	if (OverlayWidgetClass != nullptr && OverlayWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget = Cast<UHawnmunUserWidget>(Widget);

		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();

		OverlayWidget->AddToViewport();
	}

	if (AttributeWidgetClass != nullptr && AttributeWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeWidgetClass);
		AttributeWidget = Cast<UHawnmunUserWidget>(Widget);

		UAttributeWidgetController* WidgetController = GetAttributeWidgetController(WidgetControllerParams);

		AttributeWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();

		AttributeWidget->AddToViewport();
		
		AttributeWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		AttributeWidget->SetPositionInViewport(FVector2D(ViewportSize.X - 50.f, ViewportSize.Y * 0.5f), true);
		
		AttributeWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InventoryWidgetClass != nullptr && InventoryWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget = Cast<UHawnmunUserWidget>(Widget);

		UInventoryWidgetController* WidgetController = GetInventoryWidgetController(WidgetControllerParams);

		InventoryWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();

		InventoryWidget->AddToViewport();
		
		InventoryWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		InventoryWidget->SetPositionInViewport(FVector2D(ViewportSize.X - 50.f, ViewportSize.Y * 0.5f), true);
		
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		if (PlayerInfoWidgetClass != nullptr)
		{
			UUserWidget* InfoWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerInfoWidgetClass);
			PlayerInfoWidget = Cast<UHawnmunUserWidget>(InfoWidget);
			PlayerInfoWidget->SetWidgetController(WidgetController);

			PlayerInfoWidget->AddToViewport();

			PlayerInfoWidget->SetAlignmentInViewport(FVector2D(0.0f, 0.5f));
			PlayerInfoWidget->SetPositionInViewport(FVector2D(50.f, ViewportSize.Y * 0.5f), true);

			PlayerInfoWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (DungeonWidgetClass != nullptr && DungeonWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), DungeonWidgetClass);
		DungeonWidget = Cast<UHawnmunUserWidget>(Widget);

		UDungeonMapWidgetController* WidgetController = GetDungeonWidgetController(WidgetControllerParams);

		DungeonWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();
	}

	if (SaveSlotWidgetClass != nullptr && SaveSlotWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SaveSlotWidgetClass);
		SaveSlotWidget = Cast<UHawnmunUserWidget>(Widget);

		USaveSlotWidgetController* WidgetController = GetSaveSlotWidgetController(WidgetControllerParams);

		SaveSlotWidget->SetWidgetController(WidgetController);
		
		SaveSlotWidget->AddToViewport();

		SaveSlotWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		SaveSlotWidget->SetPositionInViewport(FVector2D(ViewportSize.X - 50.f, ViewportSize.Y * 0.5f), true);
		
		SaveSlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CinematicWidgetClass != nullptr && CinematicWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CinematicWidgetClass);
		CinematicWidget = Cast<UHawnmunUserWidget>(Widget);

		UCinematicWidgetController* WidgetController = GetCinematicSlotWidgetController(WidgetControllerParams);

		CinematicWidget->SetWidgetController(WidgetController);
	}

	if (MissionClearWidgetClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MissionClearWidgetClass);
		MissionClearWidget = Cast<UHawnmunUserWidget>(Widget);
	}

	if (PauseMenuWidgetClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), PauseMenuWidgetClass);
		PauseMenuWidget = Cast<UHawnmunUserWidget>(Widget);
	}
	
	if (SettingsWidgetClass != nullptr && SettingsWidgetControllerClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SettingsWidgetClass);
		SettingsWidget = Cast<UHawnmunUserWidget>(Widget);

		USettingsWidgetController* WidgetController = GetSettingsWidgetController(WidgetControllerParams);
		
		SettingsWidget->SetWidgetController(WidgetController);
		
		SettingsWidget->AddToViewport(1);

		FVector2D CurrentViewportSize;
		GEngine->GameViewport->GetViewportSize(CurrentViewportSize);
		
		SettingsWidget->SetAlignmentInViewport(FVector2D(1.0f, 0.5f));
		SettingsWidget->SetPositionInViewport(FVector2D(CurrentViewportSize.X - 50.f, CurrentViewportSize.Y * 0.5f), true);

		SettingsWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	bWidgetsInitialized = true;
}
#pragma endregion

#pragma region Overlay Widget
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

void AHawnmunHUD::ShowOverlay() const
{
	OverlayWidget->SetVisibility(ESlateVisibility::Visible);
	OverlayWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideOverlay() const
{
	// SetVisibility는 Animation 종료 시 호출
	OverlayWidget->PlayCloseAnimation();
}
#pragma endregion

#pragma region Attribute Widget
UAttributeWidgetController* AHawnmunHUD::GetAttributeWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeWidgetController == nullptr)
	{
		AttributeWidgetController = NewObject<UAttributeWidgetController>(this, AttributeWidgetControllerClass);
		AttributeWidgetController->SetWidgetControllerParams(WCParams);
		AttributeWidgetController->BindCallbacksToDependencies();
	}
	return AttributeWidgetController;
}

void AHawnmunHUD::ShowAttribute() const
{
	AttributeWidget->SetVisibility(ESlateVisibility::Visible);
	AttributeWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideAttribute() const
{
	// SetVisibility는 Animation 종료 시 호출
	AttributeWidget->PlayCloseAnimation();
}
#pragma endregion

#pragma region Inventory Widget
UInventoryWidgetController* AHawnmunHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

void AHawnmunHUD::ShowInventory() const
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	InventoryWidgetController->ResetDescription();
	InventoryWidget->PlayOpenAnimation();

	PlayerInfoWidget->SetVisibility(ESlateVisibility::Visible);
	PlayerInfoWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideInventory() const
{
	// SetVisibility는 Animation 종료 시 호출
	InventoryWidget->PlayCloseAnimation();
	PlayerInfoWidget->PlayCloseAnimation();
}
#pragma endregion

#pragma region Dungeon Map Widget
UDungeonMapWidgetController* AHawnmunHUD::GetDungeonWidgetController(const FWidgetControllerParams& WCParams)
{
	if (DungeonWidgetController == nullptr)
	{
		DungeonWidgetController = NewObject<UDungeonMapWidgetController>(this, DungeonWidgetControllerClass);
		DungeonWidgetController->SetWidgetControllerParams(WCParams);
		DungeonWidgetController->BindCallbacksToDependencies();
	}
	return DungeonWidgetController;
}

void AHawnmunHUD::ShowDungeonMap() const
{
	DungeonWidget->AddToViewport();
	DungeonWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideDungeonMap() const
{
	// RemoveFormParent는 Animation 종료 시 호출
	DungeonWidget->PlayCloseAnimation();
}
#pragma endregion

#pragma region Save Slot Widget
USaveSlotWidgetController* AHawnmunHUD::GetSaveSlotWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SaveSlotWidgetController == nullptr)
	{
		SaveSlotWidgetController = NewObject<USaveSlotWidgetController>(this, SaveSlotWidgetControllerClass);
		SaveSlotWidgetController->SetWidgetControllerParams(WCParams);
		SaveSlotWidgetController->BindCallbacksToDependencies();
	}
	return SaveSlotWidgetController;
}

void AHawnmunHUD::ShowSaveSlot() const
{
	SaveSlotWidget->SetVisibility(ESlateVisibility::Visible);
	SaveSlotWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideSaveSlot() const
{
	// SetVisibility는 Animation 종료 시 호출
	SaveSlotWidget->PlayCloseAnimation();
}

UCinematicWidgetController* AHawnmunHUD::GetCinematicSlotWidgetController(const FWidgetControllerParams& WCParams)
{
	if (CinematicWidgetController == nullptr)
	{
		CinematicWidgetController = NewObject<UCinematicWidgetController>(this, CinematicWidgetControllerClass);
		CinematicWidgetController->SetWidgetControllerParams(WCParams);
		CinematicWidgetController->BindCallbacksToDependencies();
	}
	return CinematicWidgetController;
}

void AHawnmunHUD::ShowCinematicWidget() const
{
	CinematicWidget->AddToViewport();
}

void AHawnmunHUD::HideCinematicWidget() const
{
	CinematicWidget->RemoveFromParent();
}
#pragma endregion

#pragma region Mission Clear Widget
void AHawnmunHUD::ShowMissionClearWidget() const
{
	MissionClearWidget->AddToViewport();

	// 아이템 정산 노출을 위해 임시로 메서드 사용 (새로 메서드 만드는 것은 비효율적이라 판단)
	MissionClearWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideMissionClearWidget() const
{
	MissionClearWidget->RemoveFromParent();
}

void AHawnmunHUD::ShowPauseMenu() const
{
	PauseMenuWidget->AddToViewport();
	PauseMenuWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HidePauseMenu() const
{
	PauseMenuWidget->PlayCloseAnimation();
}

USettingsWidgetController* AHawnmunHUD::GetSettingsWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SettingsWidgetController == nullptr)
	{
		SettingsWidgetController = NewObject<USettingsWidgetController>(this, SettingsWidgetControllerClass);
		SettingsWidgetController->SetWidgetControllerParams(WCParams);
	}
	return SettingsWidgetController;
}

void AHawnmunHUD::ShowSettingsWidget()
{
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
	SettingsWidget->PlayOpenAnimation();
}

void AHawnmunHUD::HideSettingsWidget()
{
	// SetVisibility는 Animation 종료 시 호출
	SettingsWidget->PlayCloseAnimation();
}
#pragma endregion
