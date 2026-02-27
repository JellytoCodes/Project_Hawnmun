// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DataAsset_StartUpDataBase.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunCharacterBase.generated.h"

class UMotionWarpingComponent;
class UBoxComponent;
class UHawnmunAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	EquippedWeapon,
	LeftHand,
	RightHand
};

UCLASS()
class PROJECTH_API AHawnmunCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AHawnmunCharacterBase();

	UFUNCTION(BlueprintPure)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

#pragma region CombatInterface
	virtual void Die() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FVector GetProjectileSpawnSocketLocation_Implementation(const FName SocketName) override;

	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void ToggleCurrentCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType = EToggleDamageType::EquippedWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams CombatDamageEffectParams;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> HawnmunAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> HawnmunAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterData")
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> StartUpData;

	virtual void InitAbilityActorInfo();

	void AddCharacterAbilities() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma region CombatCollision
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Body")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Body")
	TObjectPtr<UBoxComponent> LeftHandCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Body")
	TObjectPtr<UBoxComponent> RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Body")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Body")
	FName RightHandCollisionBoxAttachBoneName;
	
	void SetToggleCollisionEnabled(const EToggleDamageType ToggleDamageType, const ECollisionEnabled::Type CurrentCollisionType) const;
	void OnHitTargetActor(AActor* HitActor);

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	EToggleDamageType CurrentDamageType = EToggleDamageType::EquippedWeapon;
#pragma endregion
};
