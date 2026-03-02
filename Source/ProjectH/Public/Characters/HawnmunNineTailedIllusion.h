// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunNineTailedIllusion.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTH_API AHawnmunNineTailedIllusion : public ACharacter
{
	GENERATED_BODY()

public:
	AHawnmunNineTailedIllusion();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActivateCharmIllusionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeactivateCharmIllusionEffect();

	FDamageEffectParams CombatDamageEffectParams;

	void CauseDamage();
	void AddIgnoreActor(AActor* InActor);

private :
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> CharmIllusionEffect;

	TArray<AActor*> ActorsToIgnore;
};
