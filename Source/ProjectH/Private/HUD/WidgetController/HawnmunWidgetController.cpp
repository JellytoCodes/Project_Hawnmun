// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/HawnmunWidgetController.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Controllers/HawnmunPlayerController.h"
#include "GameMode/HawnmunPlayerState.h"

#pragma region Initialization And Core Functions
void UHawnmunWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
	PlayerState = WCParams.PlayerState;
}
#pragma endregion

#pragma region Custom Widget Data And Getters
AHawnmunPlayerController* UHawnmunWidgetController::GetHawnmunPC()
{
	if (HawnmunPlayerController == nullptr)
		HawnmunPlayerController = Cast<AHawnmunPlayerController>(PlayerController);

	return HawnmunPlayerController;
}

UHawnmunAbilitySystemComponent* UHawnmunWidgetController::GetHawnmunASC()
{
	if (HawnmunAbilitySystemComponent == nullptr)
		HawnmunAbilitySystemComponent = Cast<UHawnmunAbilitySystemComponent>(AbilitySystemComponent);

	return HawnmunAbilitySystemComponent;
}

UHawnmunAttributeSet* UHawnmunWidgetController::GetHawnmunAS()
{
	if (HawnmunAttributeSet == nullptr)
		HawnmunAttributeSet = Cast<UHawnmunAttributeSet>(AttributeSet);

	return HawnmunAttributeSet;
}

AHawnmunPlayerState* UHawnmunWidgetController::GetHawnmunPS()
{
	if (HawnmunPlayerState == nullptr)
		HawnmunPlayerState = Cast<AHawnmunPlayerState>(PlayerState);

	return HawnmunPlayerState;
}
#pragma endregion