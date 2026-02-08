// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunPlayer.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

AHawnmunPlayer::AHawnmunPlayer()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->PerchRadiusThreshold = 15.0f;
	GetCharacterMovement()->LedgeCheckThreshold = 6.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 4000.f, 0.f);

	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->bConstrainToPlane = true;

	HawnmunAbilitySystemComponent = CreateDefaultSubobject<UHawnmunAbilitySystemComponent>("AbilitySystemComponent");
	HawnmunAttributeSet = CreateDefaultSubobject<UHawnmunAttributeSet>("AttributeSet");
}

void AHawnmunPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AHawnmunPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}