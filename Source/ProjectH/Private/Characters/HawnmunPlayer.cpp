// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunPlayer.h"

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Controllers/HawnmunPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/HawnmunHUD.h"

AHawnmunPlayer::AHawnmunPlayer()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HawnmunAbilitySystemComponent = CreateDefaultSubobject<UHawnmunAbilitySystemComponent>("AbilitySystemComponent");
	HawnmunAttributeSet = CreateDefaultSubobject<UHawnmunAttributeSet>("AttributeSet");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 350.f;
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 80.f);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}


int32 AHawnmunPlayer::GetCharacterLevel_Implementation()
{
	return 1;
}

void AHawnmunPlayer::Die()
{
	DieEvent();
}

void AHawnmunPlayer::EnableMappingContext_Implementation()
{
	PlayerController->EnableDefaultMappingContext();

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController->GetHUD()))
		HawnmunHUD->ShowOverlay();
}

void AHawnmunPlayer::DisableMappingContext_Implementation()
{
	PlayerController->DisableDefaultMappingContext();

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController->GetHUD()))
		HawnmunHUD->HideOverlay();
}

void AHawnmunPlayer::InitAbilityActorInfo()
{
	if (!PlayerController.IsValid()) return;

	if (AHawnmunHUD* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController.Get()->GetHUD()))
		HawnmunHUD->InitOverlay(PlayerController.Get(), HawnmunAbilitySystemComponent, HawnmunAttributeSet);
}

void AHawnmunPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (StaminaRegen && HawnmunAbilitySystemComponent)
	{
		const UGameplayEffect* staminaRegen = StaminaRegen.GetDefaultObject();
		HawnmunAbilitySystemComponent->ApplyGameplayEffectToSelf(staminaRegen, 1, HawnmunAbilitySystemComponent->MakeEffectContext());
	}
	InitAbilityActorInfo();
}

void AHawnmunPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!PlayerController.IsValid())
		PlayerController = Cast<AHawnmunPlayerController>(NewController);

	AddCharacterAbilities();
}
