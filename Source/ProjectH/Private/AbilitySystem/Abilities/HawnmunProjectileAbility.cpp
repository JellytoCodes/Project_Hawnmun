// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/Abilities/HawnmunProjectileAbility.h"

#include "Actors/HawnmunProjectileActor.h"
#include "Interfaces/CombatInterface.h"

void UHawnmunProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FName SocketName, bool bOverridePitch, float PitchOverride)
{
	const FVector SocketLocation = ICombatInterface::Execute_GetProjectileSpawnSocketLocation(GetAvatarActorFromActorInfo(), SocketName);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AHawnmunProjectileActor* ProjectileActor = GetWorld()->SpawnActorDeferred<AHawnmunProjectileActor>
	(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	ProjectileActor->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	ProjectileActor->FinishSpawning(SpawnTransform);
}
