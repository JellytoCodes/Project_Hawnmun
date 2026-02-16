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

void AHawnmunCharacterBase::InitAbilityActorInfo()
{

}

void AHawnmunCharacterBase::AddCharacterAbilities() const
{
	if (!StartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
		{
			UHawnmunAbilitySystemComponent* HawnmunASC = CastChecked<UHawnmunAbilitySystemComponent>(HawnmunAbilitySystemComponent);

			LoadedData->InitializeGameplayEffect(HawnmunASC, StartUpCharacterName, 1);
			
			HawnmunASC->AddCharacterActivateAbilities(LoadedData->StartUpOffensiveAbilities);
			HawnmunASC->AddCharacterPassiveAbilities(LoadedData->StartUpPassiveAbilities);
		}
	}	
}

void AHawnmunCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HawnmunAbilitySystemComponent)	HawnmunAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

