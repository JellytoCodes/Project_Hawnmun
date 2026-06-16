// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunEnumTypes.h"
#include "Types/HawnmunStructTypes.h" // EInteractableList가 정의된 헤더 가정
#include "InteractProp.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPropInteractedSignature);

UCLASS()
class PROJECTH_API AInteractProp : public AActor
{
	GENERATED_BODY()
	
#pragma region Core And Initialization
public:	
	AInteractProp();

protected:
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Interaction Logic
public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TriggerInteract(AActor* Interactor);

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPropInteractedSignature OnPropInteractedDelegate;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FText GetInteractMessageText() const;

private:
	bool bIsInteracted = false;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	EInteractableList InteractableList = EInteractableList::Interact; 
#pragma endregion

#pragma region Components And VFX
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> InteractCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UWidgetComponent> InteractMessageWidget;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraComponent> FireNiagara;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> InteractSound;
#pragma endregion
};