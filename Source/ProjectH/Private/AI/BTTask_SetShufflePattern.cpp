#include "AI/BTTask_SetShufflePattern.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Algo/RandomShuffle.h"

UBTTask_SetShufflePattern::UBTTask_SetShufflePattern()
{
	NodeName = "Set Shuffle Pattern";
	bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTTask_SetShufflePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (BasePatternBag.IsEmpty()) return EBTNodeResult::Failed;

	if (CurrentBag.IsEmpty())
	{
		if (LastPattern != -1)
		{
			LastPattern = -1;
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(PatternIndexKey.SelectedKeyName, -1);
			return EBTNodeResult::Succeeded;
		}
		
		CurrentBag = BasePatternBag;
		Algo::RandomShuffle(CurrentBag);
	}

	int32 DrawnPattern = CurrentBag.Pop();

	if (DrawnPattern == LastPattern && CurrentBag.Num() > 0)
	{
		int32 SwapIdx = -1;
		
		for (int32 i = 0; i < CurrentBag.Num(); ++i)
		{
			if (CurrentBag[i] != LastPattern)
			{
				SwapIdx = i;
				break;
			}
		}

		if (SwapIdx != -1)
		{
			int32 Temp = DrawnPattern;
			DrawnPattern = CurrentBag[SwapIdx];
			CurrentBag[SwapIdx] = Temp;

			Algo::RandomShuffle(CurrentBag);
		}
	}

	LastPattern = DrawnPattern;
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(PatternIndexKey.SelectedKeyName, DrawnPattern);

	return EBTNodeResult::Succeeded;
}