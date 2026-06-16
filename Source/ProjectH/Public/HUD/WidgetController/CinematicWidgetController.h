// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HawnmunWidgetController.h"
#include "CinematicWidgetController.generated.h"

class UMediaSource;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayCinematicSignature, FGameplayTag, CinematicTag);

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UCinematicWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Cinematic|Control")
	void RequestPlayCinematic(FGameplayTag CinematicTag, AActor* InActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Cinematic|Control")
	void OnCinematicFinished();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cinematic|Media")
	UMediaSource* GetMediaSourceByTag(const FGameplayTag& Tag) const;

	UPROPERTY(BlueprintAssignable, Category = "Cinematic|Event")
	FOnPlayCinematicSignature OnPlayCinematicDelegate;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> CachedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Cinematic|Media")
	TMap<FGameplayTag, UMediaSource*> CinematicMedium;

	FGameplayTag CachedCinematicTag;
};