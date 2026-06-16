// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/FoxFireDropActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "Interfaces/PlayerInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectH/ProjectH.h"

AFoxFireDropActor::AFoxFireDropActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	WarningEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WarningEffect"));
	WarningEffect->SetupAttachment(RootComp);
	WarningEffect->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	WarningEffect->SetAutoActivate(false);

	FallingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("FallingCollision"));
	FallingCollision->SetupAttachment(RootComp);
	FallingCollision->SetSphereRadius(50.f);
	
	FallingCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FallingCollision->SetCollisionObjectType(ECC_Projectile);
	FallingCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	FallingCollision->SetCollisionResponseToChannel(ECC_Environment, ECR_Block);

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileEffect"));
	ProjectileEffect->SetupAttachment(FallingCollision);
	ProjectileEffect->SetAutoActivate(false);

	ExplosionEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->SetupAttachment(FallingCollision);
	ExplosionEffect->SetAutoActivate(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->SetUpdatedComponent(FallingCollision);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bRotationFollowsVelocity = false;
}

void AFoxFireDropActor::BeginPlay()
{
	Super::BeginPlay();

	check(RootComp);
	check(WarningEffect);
	check(FallingCollision);
	check(ProjectileEffect);
	check(ExplosionEffect);
	check(ProjectileMovement);

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AFoxFireDropActor::OnProjectileStop);
}

void AFoxFireDropActor::ActiveFoxFireDrop(const FDamageEffectParams& CombatDamageEffectParams)
{
	UGameplayStatics::PlaySoundAtLocation(this, DropSound, GetActorLocation());
	CachedDamageParams = CombatDamageEffectParams;

	ProjectileMovement->Deactivate();

	WarningEffect->SetCustomTimeDilation(1.5f);
	WarningEffect->Activate(true);
	WarningEffect->SetFloatParameter(FName("Radius"), Radius);

	ProjectileEffect->Activate(true);

	FallingCollision->SetRelativeLocation(FVector(0.f, 0.f, SpawnHeightOffset));
	FallingCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ProjectileMovement->SetUpdatedComponent(FallingCollision);
	ProjectileMovement->SetComponentTickEnabled(true);

	const float SpeedZ = SpawnHeightOffset / FMath::Max(0.01f, FallingDuration);
	ProjectileMovement->Velocity = FVector(0.f, 0.f, -SpeedZ);
	
	ProjectileMovement->UpdateComponentVelocity();
	ProjectileMovement->Activate(true);
}

void AFoxFireDropActor::OnProjectileStop(const FHitResult& ImpactResult)
{
	FallingCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileEffect->Deactivate();
	WarningEffect->Deactivate();
	ExplosionEffect->Activate(true);

	OnImpact(ImpactResult.Location);

	ApplyDamage();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor(),
		HawnmunGameplayTags::Event_Combat_SkillEnd,
		FGameplayEventData()
	);
}

void AFoxFireDropActor::ApplyDamage()
{
	TArray<AActor*> OutOverlappingActors;
	
	UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(
		this,
		OutOverlappingActors,
		ActorsToIgnore,
		Radius,
		GetActorLocation());

	for (auto& Actor : OutOverlappingActors)
	{
		if (Actor->Implements<UPlayerInterface>())
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			if (!TargetASC) continue;

			const bool bIsRolling = TargetASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Rolling);

			FGameplayEventData EventData;
			EventData.Instigator = CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor();
			EventData.Target = TargetASC->GetAvatarActor();

			if (bIsRolling)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor(), HawnmunGameplayTags::Event_Invincible, EventData);
			}
			else
			{
				FDamageEffectParams Params = CachedDamageParams;
				Params.TargetAbilitySystemComponent = TargetASC;
				UHawnmunFunctionLibrary::ApplyDamageEffect(Params);
			}
		}
	}
}

void AFoxFireDropActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}