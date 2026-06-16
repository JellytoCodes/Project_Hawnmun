// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AI/BTTask_RandomActivateAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"

UBTTask_RandomActivateAbility::UBTTask_RandomActivateAbility()
{
	NodeName = "Random Activate Ability";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_RandomActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AbilityTags.IsEmpty()) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	bTaskIsExecuting = true;
	bAbilityFinishedSynchronously = false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn()) return EBTNodeResult::Failed;

	CachedASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (!CachedASC) return EBTNodeResult::Failed;

	int32 RandomIndex = FMath::RandRange(0, AbilityTags.Num() - 1);
	SelectedTag = AbilityTags[RandomIndex];

	OnAbilityEndedDelegateHandle = CachedASC->OnAbilityEnded.AddUObject(this, &UBTTask_RandomActivateAbility::OnAbilityEnded);

	bool bActivated = CachedASC->TryActivateAbilitiesByTag(FGameplayTagContainer(SelectedTag));

	bTaskIsExecuting = false;

	if (!bActivated)
	{
		CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
		return EBTNodeResult::Failed;
	}

	if (bAbilityFinishedSynchronously)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RandomActivateAbility::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	// 실행 시점에 캐싱해둔 SelectedTag와 비교하여 해당 능력이 끝났는지 검증
	if (AbilityEndedData.AbilityThatEnded && AbilityEndedData.AbilityThatEnded->AbilityTags.HasTag(SelectedTag))
	{
		if (CachedASC)
		{
			CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
		}

		if (bTaskIsExecuting)
		{
			bAbilityFinishedSynchronously = true;
		}
		else if (CachedOwnerComp)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

FString UBTTask_RandomActivateAbility::GetStaticDescription() const
{
	if (!AbilityTags.IsEmpty())
	{
		FString TagList = TEXT("Random Tags:\n");
		for (const FGameplayTag& Tag : AbilityTags)
		{
			TagList += FString::Printf(TEXT("- [%s]\n"), *Tag.ToString());
		}
		return TagList;
	}
	
	return FString(TEXT("Warning: No Ability Tags Selected!"));
}

EBTNodeResult::Type UBTTask_RandomActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedASC)
	{
		CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_RandomActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (CachedASC)
	{
		CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}