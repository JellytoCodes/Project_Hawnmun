#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h" // UBTTaskNode »ó¼Ó
#include "BTTask_SetShufflePattern.generated.h"

UCLASS()
class PROJECTH_API UBTTask_SetShufflePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetShufflePattern();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatternIndexKey;

	UPROPERTY(EditAnywhere, Category = "Pattern")
	TArray<int32> BasePatternBag;

private:
	UPROPERTY(Transient)
	TArray<int32> CurrentBag;

	UPROPERTY(Transient)
	int32 LastPattern = -1;
};