// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceLocation.generated.h"

struct FRotateToFaceLocationTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	FVector TargetLocation = FVector::ZeroVector;
	bool bHasTarget = false;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && bHasTarget;
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetLocation = FVector::ZeroVector;
		bHasTarget = false;
	}
};

UCLASS()
class PROJECTH_API UBTTask_RotateToFaceLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RotateToFaceLocation();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool HasReachedAnglePrecision(const APawn* QueryPawn, const FVector& TargetLocation) const;

	UPROPERTY(EditAnywhere, Category = "Face Location")
	float AnglePrecision = 10.f;

	UPROPERTY(EditAnywhere, Category = "Face Location")
	float RotationInterpSpeed = 5.f;

	// 캐릭터라면 보통 Yaw만 도는 게 자연스러움
	UPROPERTY(EditAnywhere, Category = "Face Location")
	bool bYawOnly = true;

	UPROPERTY(EditAnywhere, Category = "Face Location")
	FBlackboardKeySelector InLocationToFaceKey;
};
