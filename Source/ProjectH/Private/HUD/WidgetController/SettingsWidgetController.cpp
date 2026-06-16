// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/SettingsWidgetController.h"
#include "GameFramework/GameUserSettings.h"

#pragma region Graphics Settings
int32 USettingsWidgetController::GetGraphicsQuality(EGraphicsTypes Type) const
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings) return 0;

	switch (Type)
	{
		case EGraphicsTypes::ViewDistance:       return Settings->GetViewDistanceQuality();
		case EGraphicsTypes::AntiAliasing:       return Settings->GetAntiAliasingQuality();
		case EGraphicsTypes::PostProcessing:     return Settings->GetPostProcessingQuality();
		case EGraphicsTypes::Shadows:            return Settings->GetShadowQuality();
		case EGraphicsTypes::GlobalIllumination: return Settings->GetGlobalIlluminationQuality();
		case EGraphicsTypes::Reflections:        return Settings->GetReflectionQuality();
		case EGraphicsTypes::Texture:            return Settings->GetTextureQuality();
		case EGraphicsTypes::Effects:            return Settings->GetVisualEffectQuality();
		case EGraphicsTypes::Foliage:            return Settings->GetFoliageQuality();
		case EGraphicsTypes::Shading:            return Settings->GetShadingQuality();
	}
	
	return 0;
}

void USettingsWidgetController::SetGraphicsQuality(EGraphicsTypes Type, int32 QualityLevel)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings) return;

	switch (Type)
	{
		case EGraphicsTypes::ViewDistance:       Settings->SetViewDistanceQuality(QualityLevel); break;
		case EGraphicsTypes::AntiAliasing:       Settings->SetAntiAliasingQuality(QualityLevel); break;
		case EGraphicsTypes::PostProcessing:     Settings->SetPostProcessingQuality(QualityLevel); break;
		case EGraphicsTypes::Shadows:            Settings->SetShadowQuality(QualityLevel); break;
		case EGraphicsTypes::GlobalIllumination: Settings->SetGlobalIlluminationQuality(QualityLevel); break;
		case EGraphicsTypes::Reflections:        Settings->SetReflectionQuality(QualityLevel); break;
		case EGraphicsTypes::Texture:            Settings->SetTextureQuality(QualityLevel); break;
		case EGraphicsTypes::Effects:            Settings->SetVisualEffectQuality(QualityLevel); break;
		case EGraphicsTypes::Foliage:            Settings->SetFoliageQuality(QualityLevel); break;
		case EGraphicsTypes::Shading:            Settings->SetShadingQuality(QualityLevel); break;
	}
}
#pragma endregion

#pragma region Audio Settings
void USettingsWidgetController::SetMasterVolume(float Volume)
{
	// 사운드 클래스 볼륨 조절 로직 구현 영역
}
#pragma endregion

#pragma region System And Apply
void USettingsWidgetController::ApplySettings()
{
	if (UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings())
	{
		Settings->ApplySettings(false);
		Settings->SaveSettings();
	}
}
#pragma endregion