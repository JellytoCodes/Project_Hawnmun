// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Controllers/HawnmunPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Input/HawnmunEnhancedInputComponent.h"

AHawnmunPlayerController::AHawnmunPlayerController()
{

}

void AHawnmunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerContext);

	check(InputConfig);

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}
}

void AHawnmunPlayerController::SetupInactiveStateInputComponent(UInputComponent* InComponent)
{
	Super::SetupInactiveStateInputComponent(InComponent);

	UHawnmunEnhancedInputComponent* HawnmunInputComponent = CastChecked<UHawnmunEnhancedInputComponent>(InputComponent);

	HawnmunInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHawnmunPlayerController::Move);

	HawnmunInputComponent->BindAbilityActions(InputConfig, this, 
		&ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AHawnmunPlayerController::Move(const FInputActionValue& value)
{

}

void AHawnmunPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//if (GetASC() && GetASC()->HasMatchingGameplayTag())
}

void AHawnmunPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{

}

void AHawnmunPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

}

UHawnmunAbilitySystemComponent* AHawnmunPlayerController::GetASC()
{
	if (HawnmunAbilitySystemComponent == nullptr)
	{
		HawnmunAbilitySystemComponent = Cast<UHawnmunAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HawnmunAbilitySystemComponent.Get();
}
