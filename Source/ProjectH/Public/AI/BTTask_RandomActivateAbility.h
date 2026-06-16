// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BTTask_RandomActivateAbility.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UBTTask_RandomActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomActivateAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<FGameplayTag> AbilityTags;

private:
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	UPROPERTY(Transient)
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	FDelegateHandle OnAbilityEndedDelegateHandle;

	FGameplayTag SelectedTag; 

	bool bTaskIsExecuting = false;
	bool bAbilityFinishedSynchronously = false;
};