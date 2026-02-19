// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"

UHawnmunAbilitySystemComponent* UHawnmunFunctionLibrary::NativeGetHawnmunASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UHawnmunAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UHawnmunFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd)) 
		ASC->AddLooseGameplayTag(TagToAdd);
}

void UHawnmunFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove)) 
		ASC->RemoveLooseGameplayTag(TagToRemove);
}

bool UHawnmunFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UHawnmunAbilitySystemComponent* ASC = NativeGetHawnmunASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UHawnmunFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EHawnmunConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EHawnmunConfirmType::Yes : EHawnmunConfirmType::No;
}

bool UHawnmunFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}
