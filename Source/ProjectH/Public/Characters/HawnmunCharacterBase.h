// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HawnmunCharacterBase.generated.h"

class UHawnmunAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECTH_API AHawnmunCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHawnmunCharacterBase();

	UFUNCTION(BlueprintPure)
	UHawnmunAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> HawnmunAttributeSet;
	
	
private :

};
