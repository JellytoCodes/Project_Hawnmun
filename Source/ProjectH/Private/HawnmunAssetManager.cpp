// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "HawnmunGameplayTags.h"

UHawnmunAssetManager& UHawnmunAssetManager::Get()
{
	check(GEngine);
	UHawnmunAssetManager* AssetManager = Cast<UHawnmunAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void UHawnmunAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	HawnmunGameplayTags::InitializeNativeGameplayTags();

	// Custom GameplayEffectContext를 사용하는 경우 반드시 글로벌 초기화 필요
	UAbilitySystemGlobals::Get().InitGlobalData();
}