// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Controllers/HawnmunAIController.h"

#include "Characters/HawnmunEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "HUD/HawnmunHUD.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AHawnmunAIController::AHawnmunAIController()
{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("AISenseConfig_Sight");

	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	AISenseConfig_Sight->SightRadius = 300.f;
	AISenseConfig_Sight->LoseSightRadius = 350.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AHawnmunAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AHawnmunAIController::SetPerceptionEnabled(bool bEnabled)
{
	checkf(EnemyPerceptionComponent, TEXT("SetPerceptionEnabled failed: EnemyPerceptionComponent is null."));

	if (!bEnabled)
	{
		EnemyPerceptionComponent->ForgetAll();
	}

	EnemyPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), bEnabled);
	bEnabled ? EnemyPerceptionComponent->Activate(true) : EnemyPerceptionComponent->Deactivate();
	EnemyPerceptionComponent->SetComponentTickEnabled(bEnabled);
	EnemyPerceptionComponent->RequestStimuliListenerUpdate();
}

void AHawnmunAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed() || !Actor) return;

	AHawnmunEnemy* ControlledEnemy = Cast<AHawnmunEnemy>(GetPawn());
	if (!ControlledEnemy || !ControlledEnemy->IsEnemyActive() || ControlledEnemy->IsDead_Implementation()) return;

	UBlackboardComponent* BBComp = GetBlackboardComponent();
	if (!BBComp || BBComp->GetValueAsObject(TEXT("TargetActor"))) return;

	BBComp->SetValueAsObject(TEXT("TargetActor"), Actor);

	if (auto* GM = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->AddAggroEnemy(ControlledEnemy);
	}

	FString BossName = TEXT("");

	if (ControlledEnemy->ActorHasTag(FName("NineTailed")))
	{
		BossName = TEXT("홍월요");
	}
	else if (ControlledEnemy->ActorHasTag(FName("WildBoar")))
	{
		BossName = TEXT("편각야돈");
	}

	if (BossName.IsEmpty()) return;

	APlayerController* PC = Cast<APlayerController>(Actor->GetInstigatorController());
	AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD());
	UOverlayWidgetController* OverlayWC = HUD->GetOverlayWidgetController(FWidgetControllerParams());

	OverlayWC->BindBossAttributes(ControlledEnemy, BossName);
}
