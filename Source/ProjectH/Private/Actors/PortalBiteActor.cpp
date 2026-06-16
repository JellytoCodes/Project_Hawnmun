#include "Actors/PortalBiteActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "HawnmunFunctionLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "HawnmunGameplayTags.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

APortalBiteActor::APortalBiteActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TelegraphEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TelegraphEffect"));
	TelegraphEffect->SetupAttachment(RootComponent);
	TelegraphEffect->SetAutoActivate(false);

	BiteEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BiteEffect"));
	BiteEffect->SetupAttachment(RootComponent);
	BiteEffect->SetAutoActivate(false);
}

void APortalBiteActor::ApplyDamage()
{
	UGameplayStatics::PlaySoundAtLocation(this, BiteSound, GetActorLocation());

	BiteEffect->Activate(true);
	TelegraphEffect->Deactivate();

	TArray<AActor*> OutOverlappingActors;
	
	UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(
		this,
		OutOverlappingActors,
		ActorsToIgnore,
		DamageRadius,
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

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		CachedDamageParams.SourceAbilitySystemComponent->GetAvatarActor(),
		HawnmunGameplayTags::Event_Combat_SkillEnd,
		FGameplayEventData()
	);
	
}

void APortalBiteActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}

void APortalBiteActor::ActivatePortalBite(const FVector& TargetLocation, const FDamageEffectParams& DamageParams)
{
	UGameplayStatics::PlaySoundAtLocation(this, CastSound, GetActorLocation());

	SetActorLocation(TargetLocation);
	CachedDamageParams = DamageParams;

	TelegraphEffect->Activate(true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ApplyDamage, AttackDelay, false);
}
