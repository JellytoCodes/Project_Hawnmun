// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunEnemy.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/HawnmunAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AHawnmunEnemy::AHawnmunEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	HawnmunAbilitySystemComponent = CreateDefaultSubobject<UHawnmunAbilitySystemComponent>("AbilitySystemComponent");
	HawnmunAttributeSet = CreateDefaultSubobject<UHawnmunAttributeSet>("AttributeSet");
}

void AHawnmunEnemy::SetSpawnedActorByBlackBoardKey(AActor* spawnedActor)
{
		if (UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject("SpawnedActor", spawnedActor);
	}
}

int32 AHawnmunEnemy::GetCharacterLevel_Implementation()
{
	return 1;
}

void AHawnmunEnemy::Die()
{
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		//Weapon->AddImpulse();
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//GetMesh()->AddImpulse()

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHawnmunEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}

void AHawnmunEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	HawnmunAIController = Cast<AHawnmunAIController>(NewController);
	if (BehaviorTree && HawnmunAIController)
	{
		HawnmunAIController->RunBehaviorTree(BehaviorTree);

		if (UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsFloat("DefaultMaxWalkSpeed", GetCharacterMovement()->MaxWalkSpeed);
		}
	}

	if (HawnmunAbilitySystemComponent)
		AddCharacterAbilities();
}
