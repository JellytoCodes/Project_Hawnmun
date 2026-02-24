// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/HawnmunAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetHawnmunAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetHawnmunAS()->GetMaxHealth());
	OnStaminaChanged.Broadcast(GetHawnmunAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetHawnmunAS()->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetHealthAttribute()).
	AddLambda([&](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetMaxHealthAttribute()).
	AddLambda([&](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetStaminaAttribute()).
	AddLambda([&](const FOnAttributeChangeData& Data)
	{
		OnStaminaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetMaxStaminaAttribute()).
	AddLambda([&](const FOnAttributeChangeData& Data)
	{
		OnMaxStaminaChanged.Broadcast(Data.NewValue);
	});
}
