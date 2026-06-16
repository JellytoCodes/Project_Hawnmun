// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "SettingsWidgetController.generated.h"

UENUM(BlueprintType)
enum class EGraphicsTypes : uint8
{
	ViewDistance,
	AntiAliasing,
	PostProcessing,
	Shadows,
	GlobalIllumination,
	Reflections,
	Texture,
	Effects,
	Foliage,
	Shading
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API USettingsWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

#pragma region Graphics Settings
public:
	UFUNCTION(BlueprintCallable, Category = "Settings|Graphics")
	int32 GetGraphicsQuality(EGraphicsTypes Type) const;

	UFUNCTION(BlueprintCallable, Category = "Settings|Graphics")
	void SetGraphicsQuality(EGraphicsTypes Type, int32 QualityLevel);
#pragma endregion

#pragma region Audio Settings
public:
	UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
	void SetMasterVolume(float Volume);
#pragma endregion

#pragma region System And Apply
public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplySettings();
#pragma endregion
};