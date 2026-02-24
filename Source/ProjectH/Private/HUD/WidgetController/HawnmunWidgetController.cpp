// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/HawnmunWidgetController.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Controllers/HawnmunPlayerController.h"

void UHawnmunWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

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
