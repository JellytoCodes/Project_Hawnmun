// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/OpenWidgetActor.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "ProjectH/ProjectH.h"

AOpenWidgetActor::AOpenWidgetActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(CollisionBox);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());


	InteractableMessageWidget = CreateDefaultSubobject<UWidgetComponent>("InteractableMessageWidget");
	InteractableMessageWidget->SetupAttachment(GetRootComponent());
	InteractableMessageWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractableMessageWidget->SetDrawAtDesiredSize(true);
	InteractableMessageWidget->SetVisibility(false);
}

void AOpenWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractableMessageWidget)
	{
		UUserWidget* WidgetObj = InteractableMessageWidget->GetUserWidgetObject();
		if (WidgetObj)
		{
			UTextBlock* MessageText = Cast<UTextBlock>(WidgetObj->GetWidgetFromName(TEXT("MessageText")));
			if (MessageText)
			{
				MessageText->SetText(GetInteractMessageText());
			}
		}
	}

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void AOpenWidgetActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, true, InteractableList, nullptr);

		if (InteractableMessageWidget)
		{
			InteractableMessageWidget->SetVisibility(true);
		}
	}
}

void AOpenWidgetActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, false, EInteractableList::None, nullptr);

		if (InteractableMessageWidget)
		{
			InteractableMessageWidget->SetVisibility(false);
		}
	}
}

FText AOpenWidgetActor::GetInteractMessageText() const
{
	switch (InteractableList)
	{
	case EInteractableList::AttributeWidget:
		return FText::FromString(TEXT("스탯 창 열기 (F)"));
	case EInteractableList::InventoryWidget:
		return FText::FromString(TEXT("인벤토리 열기 (F)"));
	case EInteractableList::SaveWidget:
		return FText::FromString(TEXT("세이브 창 열기 (F)"));
	case EInteractableList::Loot:
		return FText::FromString(TEXT("루팅하기 (F)"));
	default:
		return FText::FromString(TEXT("상호작용 (F)"));
	}
}