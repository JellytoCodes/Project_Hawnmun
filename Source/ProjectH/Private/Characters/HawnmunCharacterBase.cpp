// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunCharacterBase.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"

AHawnmunCharacterBase::AHawnmunCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

UHawnmunAbilitySystemComponent* AHawnmunCharacterBase::GetAbilitySystemComponent() const
{
	return CastChecked<UHawnmunAbilitySystemComponent>(HawnmunAbilitySystemComponent);
}

void AHawnmunCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHawnmunCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHawnmunCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHawnmunCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HawnmunAbilitySystemComponent)	HawnmunAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

