// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/CinematicWidgetController.h"

#include "HawnmunGameplayTags.h"
#include "Characters/HawnmunPlayer.h"
#include "Interfaces/EnemyInterface.h"
#include "Controllers/HawnmunPlayerController.h"
#include "MediaSource.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "HUD/HawnmunHUD.h"
#include "Kismet/GameplayStatics.h"

void UCinematicWidgetController::RequestPlayCinematic(FGameplayTag CinematicTag, AActor* InActor)
{
	CachedActor = InActor;
	CachedCinematicTag = CinematicTag;

	if (AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->StopBGM(0.5f);
	}

	if (PlayerController) 
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_DisableMappingContext(PlayerPawn);
		}
	}

	OnPlayCinematicDelegate.Broadcast(CinematicTag);
}

void UCinematicWidgetController::OnCinematicFinished()
{
	AHawnmunPlayerController* HawnmunPC = GetHawnmunPC();
	checkf(HawnmunPC, TEXT("OnCinematicFinished: HawnmunPlayerController is null."));

	HawnmunPC->HoldBlackScreen();

	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));

	checkf(GameMode, TEXT("OnCinematicFinished: HawnmunGameModeBase is null."));

	const bool bIsEnemyCinematic = CachedActor.IsValid() && CachedActor->Implements<UEnemyInterface>();

	if (CachedActor.IsValid() && CachedActor->Implements<UEnemyInterface>())
	{
		// 구미호 페이즈 전환 및 보스 처치 후처리
		IEnemyInterface::Execute_ResumeEnemyAI(CachedActor.Get());
	}
	else if (CachedCinematicTag.MatchesTagExact(HawnmunGameplayTags::Cinematic_WildBoar_Encounter))
	{
		GameMode->ChangeBGMByTag(HawnmunGameplayTags::BGM_WildBoar_Battle, 0.5f, 0.f);
	}
	else if (CachedCinematicTag.MatchesTagExact(HawnmunGameplayTags::Cinematic_NineTailed_Encounter))
	{
		GameMode->ChangeBGMByTag(HawnmunGameplayTags::BGM_NineTailed_Phase1, 0.5f, 0.f);
	}
	else
	{
		// 장벽 파괴 등 일반 상호작용 시네마틱
		GameMode->ChangeBGMByTag(HawnmunGameplayTags::BGM_CombatField_Default, 0.5f, 0.f);
	}

	CachedActor.Reset();
	CachedCinematicTag = FGameplayTag();

	APawn* PlayerPawn = HawnmunPC->GetPawn();
	checkf(PlayerPawn, TEXT("OnCinematicFinished: PlayerPawn is null."));

	if (PlayerPawn->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_EnableMappingContext(PlayerPawn);
	}

	AHawnmunHUD* HUD = Cast<AHawnmunHUD>(HawnmunPC->GetHUD());
	checkf(HUD, TEXT("OnCinematicFinished: HawnmunHUD is null."));

	HUD->HideCinematicWidget();

	HawnmunPC->SetCinematicInputBlocked(false);
	HawnmunPC->FadeInFromBlack(1.f);
}

UMediaSource* UCinematicWidgetController::GetMediaSourceByTag(const FGameplayTag& Tag) const
{
	return CinematicMedium.FindRef(Tag);
}