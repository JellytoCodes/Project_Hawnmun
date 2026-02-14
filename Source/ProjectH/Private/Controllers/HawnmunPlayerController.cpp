// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Controllers/HawnmunPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Input/HawnmunEnhancedInputComponent.h"
#include "ProjectH/ProjectH.h"

AHawnmunPlayerController::AHawnmunPlayerController()
{

}

void AHawnmunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = GetPawn<APawn>();

	check(PlayerContext);
	check(InputConfig);

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}
}

void AHawnmunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UHawnmunEnhancedInputComponent* HawnmunInputComponent = CastChecked<UHawnmunEnhancedInputComponent>(InputComponent);

	if (HawnmunInputComponent)
	{
		HawnmunInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		HawnmunInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		HawnmunInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void AHawnmunPlayerController::Move(const FInputActionValue& value)
{
	if (ControlledPawn == nullptr) return;

	const FVector2D MovementVector = value.Get<FVector2D>();
	const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}	
}

void AHawnmunPlayerController::Look(const FInputActionValue& value)
{
	if (ControlledPawn == nullptr) return;

	const FVector2D LookAxisVector = value.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
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
