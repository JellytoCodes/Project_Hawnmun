// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunProjectileActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class PROJECTH_API AHawnmunProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHawnmunProjectileActor();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	bool IsValidOverlap(AActor* OtherActor) const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	// Collision 형태는 각 발사체마다 다를 수 있기 때문에
	// 블루프린트에서 Collision 생성하여 Event 등록하는 방식으로 설정
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor);

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = true;

private :
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
	FGameplayTag GameplayTagCue;
};
