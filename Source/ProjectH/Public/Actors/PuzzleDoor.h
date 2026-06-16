// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleDoor.generated.h"

class AInteractProp;
class UStaticMeshComponent;

UCLASS()
class PROJECTH_API APuzzleDoor : public AActor
{
	GENERATED_BODY()
	
#pragma region Core And Initialization
public:	
	APuzzleDoor();

protected:
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Puzzle Logic
public :
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle")
	void OpenDoorEvent();

private:
	UFUNCTION()
	void OnPropInteracted();

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	TArray<TObjectPtr<AInteractProp>> LinkedProps;

	int32 TargetInteractCount = 0;
	int32 CurrentInteractCount = 0;
#pragma endregion

#pragma region Door Components
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorMesh;
#pragma endregion
};