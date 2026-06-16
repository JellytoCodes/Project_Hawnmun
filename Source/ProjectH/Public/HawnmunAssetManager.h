// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HawnmunAssetManager.generated.h"

//커스텀 에셋 매니저 등록 (태그 및 데이터 초기화 필수)
//[/Script/Engine.Engine]
//AssetManagerClassName=/Script/ProjectH.HawnmunAssetManager

UCLASS()
class PROJECTH_API UHawnmunAssetManager : public UAssetManager
{
	GENERATED_BODY()

public :
	static UHawnmunAssetManager& Get();

protected :
	virtual void StartInitialLoading() override;
};


