// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AI/BTTask_ActivateAbilityByTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"

UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	NodeName = "Activate Ability By Tag";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	bTaskIsExecuting = true;
	bAbilityFinishedSynchronously = false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn()) return EBTNodeResult::Failed;

	CachedASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (!CachedASC) return EBTNodeResult::Failed;

	OnAbilityEndedDelegateHandle = CachedASC->OnAbilityEnded.AddUObject(this, &UBTTask_ActivateAbilityByTag::OnAbilityEnded);

	bool bActivated = CachedASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));

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

void UBTTask_ActivateAbilityByTag::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.AbilityThatEnded && AbilityEndedData.AbilityThatEnded->AbilityTags.HasTag(AbilityTag))
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

FString UBTTask_ActivateAbilityByTag::GetStaticDescription() const
{
	if (AbilityTag.IsValid())
	{
		return FString::Printf(TEXT("Activate Tag:\n[%s]"), *AbilityTag.ToString());
	}
	
	return FString(TEXT("Warning: No Ability Tag Selected!"));
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedASC)
	{
		CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_ActivateAbilityByTag::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (CachedASC)
	{
		CachedASC->OnAbilityEnded.Remove(OnAbilityEndedDelegateHandle);
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}