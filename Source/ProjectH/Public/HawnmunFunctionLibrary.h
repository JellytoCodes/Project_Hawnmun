// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/HawnmunEnumTypes.h"
#include "HawnmunFunctionLibrary.generated.h"

class UHawnmunAbilitySystemComponent;

UCLASS()
class PROJECTH_API UHawnmunFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	static UHawnmunAbilitySystemComponent* NativeGetHawnmunASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Dose Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EHawnmunConfirmType& OutConfirmType);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|Hostile")
	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);
};