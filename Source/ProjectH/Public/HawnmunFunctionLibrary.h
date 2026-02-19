// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/HawnmunEnumTypes.h"
#include "Types/HawnmunStructTypes.h"
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

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(FDamageEffectParams DamageEffectParams);
	
	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|GameplayEffects")
	static void GetLivePlayersWithinCircle(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|GameplayEffects")
	static void GetLivePlayersWithinLine(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End);

};