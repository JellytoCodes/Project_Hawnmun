// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AI/BTTask_RotateToFaceLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceLocation::UBTTask_RotateToFaceLocation()
{
	NodeName = "Native Rotate to Face Target Location";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InLocationToFaceKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InLocationToFaceKey));
}

void UBTTask_RotateToFaceLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InLocationToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceLocation::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceLocationTaskMemory);
}

FString UBTTask_RotateToFaceLocation::GetStaticDescription() const
{
	const FString KeyDescription = InLocationToFaceKey.SelectedKeyName.ToString();

	return FString::Printf(
		TEXT("Smoothly rotates to face %s(Vector) Key until angle precision %.1f is reached"),
		*KeyDescription,
		AnglePrecision
	);
}

EBTNodeResult::Type UBTTask_RotateToFaceLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AICon = OwnerComp.GetAIOwner();

	if (!BB || !AICon)
	{
		return EBTNodeResult::Failed;
	}

	APawn* OwningPawn = AICon->GetPawn();
	const FVector TargetLocation = BB->GetValueAsVector(InLocationToFaceKey.SelectedKeyName);

	FRotateToFaceLocationTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceLocationTaskMemory>(NodeMemory);
	check(Memory);

	Memory->OwningPawn = OwningPawn;
	Memory->TargetLocation = TargetLocation;
	Memory->bHasTarget = true;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (HasReachedAnglePrecision(OwningPawn, TargetLocation))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceLocationTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceLocationTaskMemory>(NodeMemory);
	check(Memory);

	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = Memory->OwningPawn.Get();
	const FVector TargetLocation = Memory->TargetLocation;

	if (HasReachedAnglePrecision(Pawn, TargetLocation))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), TargetLocation);

	if (bYawOnly)
	{
		LookAtRot.Pitch = 0.f;
		LookAtRot.Roll = 0.f;
	}

	const FRotator CurrentRot = Pawn->GetActorRotation();
	const FRotator TargetRot = FMath::RInterpTo(CurrentRot, LookAtRot, DeltaSeconds, RotationInterpSpeed);

	Pawn->SetActorRotation(TargetRot);
}

bool UBTTask_RotateToFaceLocation::HasReachedAnglePrecision(const APawn* QueryPawn, const FVector& TargetLocation) const
{
	if (!QueryPawn)
	{
		return true;
	}

	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTarget = (TargetLocation - QueryPawn->GetActorLocation());

	if (OwnerToTarget.IsNearlyZero())
	{
		return true;
	}

	const FVector OwnerToTargetNormalized = OwnerToTarget.GetSafeNormal();
	float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);

	DotResult = FMath::Clamp(DotResult, -1.f, 1.f);

	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);
	return AngleDiff <= AnglePrecision;
}

