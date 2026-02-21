// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Components/BoxComponent.h"
#include "MotionWarpingComponent.h"
#include "ProjectH/ProjectH.h"

AHawnmunCharacterBase::AHawnmunCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetGenerateOverlapEvents(false);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>("WeaponCollisionBox");
	WeaponCollisionBox->SetupAttachment(Weapon);
	WeaponCollisionBox->SetCollisionObjectType(ECC_Weapon);
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

UAbilitySystemComponent* AHawnmunCharacterBase::GetAbilitySystemComponent() const
{
	return CastChecked<UHawnmunAbilitySystemComponent>(HawnmunAbilitySystemComponent);
}

void AHawnmunCharacterBase::Die()
{
	
}

void AHawnmunCharacterBase::ToggleCurrentCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType)
{
	const ECollisionEnabled::Type CurrentCollisionType = bShouldEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

	CurrentDamageType = ToggleDamageType;

	SetToggleCollisionEnabled(CurrentDamageType, CurrentCollisionType);
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

#if WITH_EDITOR
void AHawnmunCharacterBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}
#endif

void AHawnmunCharacterBase::SetToggleCollisionEnabled(const EToggleDamageType ToggleDamageType, const ECollisionEnabled::Type CurrentCollisionType) const
{
	switch (ToggleDamageType)
	{
	case EToggleDamageType::EquippedWeapon :
		WeaponCollisionBox->SetCollisionEnabled(CurrentCollisionType);
		break;

	case EToggleDamageType::LeftHand :
		LeftHandCollisionBox->SetCollisionEnabled(CurrentCollisionType);
		break;

	case EToggleDamageType::RightHand :
		RightHandCollisionBox->SetCollisionEnabled(CurrentCollisionType);
		break;
		
	default :
		break;
	}
}

void AHawnmunCharacterBase::OnHitTargetActor(AActor* HitActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);

	const bool bIsRolling = TargetASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Rolling);

	FGameplayEventData EventData;

	if (bIsRolling)
	{

	}
	else
	{

	}
}

void AHawnmunCharacterBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	
	if (OtherActor->Implements<UCombatInterface>())
	{
		APawn* HitPawn = Cast<APawn>(OtherActor);
		if (UHawnmunFunctionLibrary::IsTargetPawnHostile(this, HitPawn) == false) return;
	
		OnHitTargetActor(HitPawn);
	
		SetToggleCollisionEnabled(CurrentDamageType, ECollisionEnabled::NoCollision);
	}
}

void AHawnmunCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HawnmunAbilitySystemComponent)	HawnmunAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

