// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunEnumTypes.h"
#include "OpenWidgetActor.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class PROJECTH_API AOpenWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOpenWidgetActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private :
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InteractableMessageWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	EInteractableList InteractableList = EInteractableList::None;

	FText GetInteractMessageText() const;
};