// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"
#include "GameplayAbility_Rolling.generated.h"

UENUM(BlueprintType)
enum class ERollSection : uint8
{
	Forward,
	Backward,
	Left,
	Right,

	End
};

UCLASS()
class PROJECTH_API UGameplayAbility_Rolling : public UHawnmunGameplayAbility
{
	GENERATED_BODY()
	
public :
	UFUNCTION(BlueprintCallable)
	void ComputeRollDirectionAndDistance();

	UPROPERTY(BlueprintReadWrite)
	ERollSection RollSection;

private :
	void GetActorStartEndLocation(const FVector& CachedRollingDirection, FVector& StartLoc, FVector& EndLoc);

	UPROPERTY(EditDefaultsOnly)
	float RollingDistance;
};
