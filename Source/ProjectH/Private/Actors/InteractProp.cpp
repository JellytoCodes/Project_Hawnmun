// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/InteractProp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "NiagaraComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

#pragma region Core And Initialization
AInteractProp::AInteractProp()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	InteractCollision = CreateDefaultSubobject<USphereComponent>("InteractCollision");
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetSphereRadius(150.f);

	FireNiagara = CreateDefaultSubobject<UNiagaraComponent>("FireNiagara");
	FireNiagara->SetupAttachment(RootComponent);
	FireNiagara->SetAutoActivate(true);

	InteractMessageWidget = CreateDefaultSubobject<UWidgetComponent>("InteractMessageWidget");
	InteractMessageWidget->SetupAttachment(RootComponent);
	InteractMessageWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractMessageWidget->SetDrawAtDesiredSize(true);
	InteractMessageWidget->SetVisibility(false);
}

void AInteractProp::BeginPlay()
{
	Super::BeginPlay();

	if (InteractMessageWidget)
	{
		UUserWidget* WidgetObj = InteractMessageWidget->GetUserWidgetObject();
		if (WidgetObj)
		{
			UTextBlock* MessageText = Cast<UTextBlock>(WidgetObj->GetWidgetFromName(TEXT("MessageText")));
			if (MessageText)
			{
				MessageText->SetText(GetInteractMessageText());
			}
		}
	}

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}
#pragma endregion

#pragma region Interaction Logic
void AInteractProp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsInteracted) return;

	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, true, InteractableList, this);
		InteractMessageWidget->SetVisibility(true);
	}
}

void AInteractProp::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, false, EInteractableList::None, nullptr);
		InteractMessageWidget->SetVisibility(false);
	}
}

void AInteractProp::TriggerInteract(AActor* Interactor)
{
	if (bIsInteracted || !IsValid(Interactor)) return;

	if (Interactor->Implements<UPlayerInterface>())
	{
		bIsInteracted = true;

		InteractMessageWidget->SetVisibility(false);
		IPlayerInterface::Execute_SetIsInteractable(Interactor, false, EInteractableList::None, nullptr);
		FireNiagara->Deactivate();

		OnPropInteractedDelegate.Broadcast();

		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}
}

FText AInteractProp::GetInteractMessageText() const
{
	return FText::FromString(TEXT("불 끄기 (F)"));
}
#pragma endregion