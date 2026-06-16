// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/AttributeWidgetController.h"
#include "GameMode/HawnmunPlayerState.h"
#include "Controllers/HawnmunPlayerController.h"
#include "AbilitySystem/HawnmunAttributeSet.h"

#pragma region Core And Initialization
void UAttributeWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetHawnmunAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetHawnmunAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).
			AddLambda([this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}

	GetHawnmunPS()->OnAttributePointsChangedDelegate.
	AddLambda([this](int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});
}
#pragma endregion

#pragma region Attribute Actions
void UAttributeWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	int32 TotalPoints = GetHawnmunPS()->GetAttributePoints();
	int32 SpentPoints = 0;

	for (const auto& Pair : PendingUpgrades)
	{
		SpentPoints += Pair.Value;
	}

	if (TotalPoints - SpentPoints > 0)
	{
		PendingUpgrades.FindOrAdd(AttributeTag) += 1;

		BroadcastAttributeInfo(AttributeTag, GetHawnmunAS()->TagsToAttributes[AttributeTag]());
		AttributePointsChangedDelegate.Broadcast(TotalPoints - SpentPoints - 1);
	}
}

void UAttributeWidgetController::DowngradeAttribute(const FGameplayTag& AttributeTag)
{
	if (PendingUpgrades.Contains(AttributeTag) && PendingUpgrades[AttributeTag] > 0)
	{
		PendingUpgrades[AttributeTag] -= 1;

		int32 TotalPoints = GetHawnmunPS()->GetAttributePoints();
		int32 SpentPoints = 0;
		for (const auto& Pair : PendingUpgrades) SpentPoints += Pair.Value;

		BroadcastAttributeInfo(AttributeTag, GetHawnmunAS()->TagsToAttributes[AttributeTag]());
		AttributePointsChangedDelegate.Broadcast(TotalPoints - SpentPoints);
	}
}

void UAttributeWidgetController::ConfirmUpgrades()
{
	if (PendingUpgrades.Num() == 0) return;

	TMap<FGameplayTag, int32> UpgradesToApply = PendingUpgrades;

	PendingUpgrades.Empty();

	GetHawnmunASC()->ApplyAttributeUpgrades(UpgradesToApply);
}

void UAttributeWidgetController::CancelUpgrades()
{
	PendingUpgrades.Empty();

	BroadcastInitialValues();
	AttributePointsChangedDelegate.Broadcast(GetHawnmunPS()->GetAttributePoints());
}

void UAttributeWidgetController::CloseAttributeWidgetByButton()
{
	CancelUpgrades();

	auto* HPC = GetHawnmunPC();
	HPC->CloseCurrentWidget();
}
#pragma endregion

#pragma region Widget Delegates And Properties
void UAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	float BaseValue = Attribute.GetNumericValue(AttributeSet);
	float PendingValue = PendingUpgrades.Contains(AttributeTag) ? PendingUpgrades[AttributeTag] : 0.f;
	Info.AttributeValue = BaseValue + PendingValue;
	AttributeInfoDelegate.Broadcast(Info);
}
#pragma endregion