// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/HawnmunProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHawnmunProjectileActor::AHawnmunProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AHawnmunProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

}
void AHawnmunProjectileActor::Destroyed()
{
	Super::Destroyed();
}


bool AHawnmunProjectileActor::IsValidOverlap(AActor* OtherActor) const
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (OtherActor == this || OtherActor->IsA(AHawnmunProjectileActor::StaticClass())) return false;
	if (UHawnmunFunctionLibrary::IsTargetPawnHostile(Cast<APawn>(SourceAvatarActor), Cast<APawn>(OtherActor)) == false) return false;

	return true;
}


void AHawnmunProjectileActor::OnHit()
{
	bHit = true;
}

void AHawnmunProjectileActor::ApplyEffectToTarget(AActor* TargetActor)
{
	if (!IsValidOverlap(TargetActor)) return;
	if (!bHit) OnHit();

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const bool bIsRolling = TargetASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Rolling);

		FGameplayEventData EventData;
		EventData.Instigator = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
		EventData.Target = TargetASC->GetAvatarActor();

		if (bIsRolling)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
		}
		else
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UHawnmunFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
	if (bDestroyOnEffectApplication) Destroy();
}


