// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/HawnmunEnumTypes.h"
#include "PlayerInterface.generated.h"

class AHawnmunPlayerState;

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTH_API IPlayerInterface
{
	GENERATED_BODY()

public:
#pragma region Input
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableMappingContext();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableMappingContext();
#pragma endregion

#pragma region Player State
	UFUNCTION(BlueprintNativeEvent)
	AHawnmunPlayerState* GetHawnmunPS();
#pragma endregion

#pragma region Interaction
	UFUNCTION(BlueprintNativeEvent)
	void SetIsInteractable(const bool InIsInteractable, const EInteractableList InList, AActor* TargetActor = nullptr);
#pragma endregion

#pragma region UI
	UFUNCTION(BlueprintNativeEvent)
	void ShowDungeonMap() const;

	UFUNCTION(BlueprintNativeEvent)
	void HideDungeonMap() const;
#pragma endregion
};