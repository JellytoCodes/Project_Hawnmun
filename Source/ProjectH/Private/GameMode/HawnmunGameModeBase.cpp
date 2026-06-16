// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "GameMode/HawnmunGameModeBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Characters/HawnmunPlayer.h"
#include "GameMode/HawnmunSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameMode/HawnmunPlayerState.h"
#include "Characters/HawnmunEnemy.h"
#include "Components/InventoryComponent.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "HawnmunGameplayTags.h"

#pragma region Core And Initialization
AHawnmunGameModeBase::AHawnmunGameModeBase()
{
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMComponent->bAutoActivate = false;
	BGMComponent->bIsUISound = true;
}

void AHawnmunGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
#pragma endregion

#pragma region Level Travel
void AHawnmunGameModeBase::TravelToMap(const FString& MapName)
{
	if (!MapName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, FName(*MapName));
	}
}

void AHawnmunGameModeBase::RetryForTravelToMap()
{
	if (UHawnmunSaveGame* ProgressData = GetProgressSaveData())
	{
		FString CurrentMapName = GetWorld()->GetMapName();
		CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		UGameplayStatics::OpenLevel(this, FName(*CurrentMapName));
	}
}
#pragma endregion

#pragma region Save And Load
void AHawnmunGameModeBase::SyncSlotToProgress(const FString& SlotName, int32 SlotIndex)
{
	UHawnmunSaveGame* SlotData = Cast<UHawnmunSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	if (!SlotData)
	{
		SlotData = Cast<UHawnmunSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}

	UGameplayStatics::SaveGameToSlot(SlotData, ProgressSlotName, ProgressSlotIndex);
}

void AHawnmunGameModeBase::SyncProgressToSlot(const FString& SlotName, int32 SlotIndex)
{
	UHawnmunSaveGame* ProgressData = GetProgressSaveData();
	if (ProgressData)
	{
		UGameplayStatics::SaveGameToSlot(ProgressData, SlotName, SlotIndex);
	}
}

void AHawnmunGameModeBase::SaveProgressData()
{
	if (UHawnmunSaveGame* ProgressData = GetProgressSaveData())
	{
		FString CurrentMapName = GetWorld()->GetMapName();
		CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		PopulateSaveGameObject(ProgressData, CurrentMapName);

		UGameplayStatics::SaveGameToSlot(ProgressData, ProgressSlotName, ProgressSlotIndex);
	}
}

UHawnmunSaveGame* AHawnmunGameModeBase::GetProgressSaveData() const
{
	if (UGameplayStatics::DoesSaveGameExist(ProgressSlotName, ProgressSlotIndex))
	{
		return Cast<UHawnmunSaveGame>(UGameplayStatics::LoadGameFromSlot(ProgressSlotName, ProgressSlotIndex));
	}

	UClass* ClassToUse = SaveGameClass ? SaveGameClass.Get() : UHawnmunSaveGame::StaticClass();
	return Cast<UHawnmunSaveGame>(UGameplayStatics::CreateSaveGameObject(ClassToUse));
}

UHawnmunSaveGame* AHawnmunGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		return Cast<UHawnmunSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	}

	UClass* ClassToUse = SaveGameClass ? SaveGameClass.Get() : UHawnmunSaveGame::StaticClass();
	return Cast<UHawnmunSaveGame>(UGameplayStatics::CreateSaveGameObject(ClassToUse));
}

void AHawnmunGameModeBase::PopulateSaveGameObject(UHawnmunSaveGame* SaveGameObject, const FString& MapName)
{
	if (!SaveGameObject) return;
	SaveGameObject->LocationName = SaveGameObject->bFirstTimeLoadIn ? "HideoutMap" : MapName;
	SaveGameObject->SaveSlotStatus = Valid;
	SaveGameObject->bFirstTimeLoadIn = false;
	SaveGameObject->SaveDate = FDateTime::Now();

	if (AHawnmunPlayer* PlayerChar = Cast<AHawnmunPlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerChar))
		{
			SaveGameObject->Strength = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetStrengthAttribute());
			SaveGameObject->Intelligence = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetIntelligenceAttribute());
			SaveGameObject->Resilience = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetResilienceAttribute());
			SaveGameObject->Vigor = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetVigorAttribute());
			SaveGameObject->Health = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetHealthAttribute());
			SaveGameObject->Stamina = ASC->GetNumericAttribute(UHawnmunAttributeSet::GetStaminaAttribute());
		}

		if (AHawnmunPlayerState* PS = PlayerChar->GetPlayerState<AHawnmunPlayerState>())
		{
			SaveGameObject->PlayerLevel = PS->GetPlayerLevel();
			SaveGameObject->PlayerXP = PS->GetPlayerXP();
			SaveGameObject->AttributePoints = PS->GetAttributePoints();
			PS->SaveInventoryAndEquipment(SaveGameObject);
		}
	}
}
#pragma endregion

#pragma region Audio Management
void AHawnmunGameModeBase::PlayBGM(USoundBase* NewSound, const float FadeInTime)
{
	if (!NewSound || !BGMComponent) return;

	BGMComponent->SetSound(NewSound);

	if (FadeInTime > 0.f)
	{
		BGMComponent->FadeIn(FadeInTime, 1.f);
	}
	else
	{
		BGMComponent->Play();
	}
}

void AHawnmunGameModeBase::ChangeBGM(USoundBase* NewSound, const float FadeInTime, const float FadeOutTime)
{
	StopBGM(FadeOutTime);
	PlayBGM(NewSound, FadeInTime);
}

void AHawnmunGameModeBase::StopBGM(const float FadeOutTime)
{
	if (!BGMComponent || !BGMComponent->IsPlaying()) return;

	if (FadeOutTime > 0.f)
	{
		BGMComponent->FadeOut(FadeOutTime, 0.f);
	}
	else
	{
		BGMComponent->Stop();
	}
}

void AHawnmunGameModeBase::PlayBGMByTag(FGameplayTag BGMTag, const float FadeInTime)
{
	checkf(BGMTag.IsValid(), TEXT("PlayBGMByTag failed: BGMTag is invalid."));

	USoundBase* const* FoundBGM = BGMMap.Find(BGMTag);
	checkf(FoundBGM && *FoundBGM, TEXT("PlayBGMByTag failed: BGM not found for tag %s."), *BGMTag.ToString());

	PlayBGM(*FoundBGM, FadeInTime);
}

void AHawnmunGameModeBase::ChangeBGMByTag(FGameplayTag BGMTag, const float FadeInTime, const float FadeOutTime)
{
	checkf(BGMTag.IsValid(), TEXT("ChangeBGMByTag failed: BGMTag is invalid."));

	USoundBase* const* FoundBGM = BGMMap.Find(BGMTag);
	checkf(FoundBGM && *FoundBGM, TEXT("ChangeBGMByTag failed: BGM not found for tag %s."), *BGMTag.ToString());

	ChangeBGM(*FoundBGM, FadeInTime, FadeOutTime);
}
#pragma endregion

#pragma region Combat And Encounters
void AHawnmunGameModeBase::AddAggroEnemy(AHawnmunEnemy* Enemy)
{
	checkf(Enemy, TEXT("AddAggroEnemy failed: Enemy is null."));

	const bool bWasEmpty = ActiveEnemies.IsEmpty();
	ActiveEnemies.Add(Enemy);

	if (bWasEmpty && !IsBossEnemy(Enemy))
	{
		ChangeBGMByTag(HawnmunGameplayTags::BGM_Combat_Default, 0.5f, 0.5f);
	}
}

void AHawnmunGameModeBase::RemoveAggroEnemy(AHawnmunEnemy* Enemy)
{
	checkf(Enemy, TEXT("RemoveAggroEnemy failed: Enemy is null."));

	const bool bRemovedDeadBoss = IsBossEnemy(Enemy);

	if (ActiveEnemies.Remove(Enemy) == 0) return;

	if (ActiveEnemies.IsEmpty() && !bRemovedDeadBoss)
	{
		ChangeBGMByTag(HawnmunGameplayTags::BGM_CombatField_Default, 0.5f, 0.5f);
	}
}

void AHawnmunGameModeBase::ResetAllAggroEnemies()
{
	for (AHawnmunEnemy* Enemy : ActiveEnemies)
	{
		Enemy->ResetToSpawnPoint();
	}

	ActiveEnemies.Empty();
}

void AHawnmunGameModeBase::RequestRespawn(AController* InController)
{
	if (InController == nullptr) return;

	APawn* OldCorpse = InController->GetPawn();

	InController->UnPossess();

	if (OldCorpse) OldCorpse->Destroy();

	if (auto* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		if (DungeonSubsystem->bHasCheckpoint)
		{
			RestartPlayerAtTransform(InController, DungeonSubsystem->CurrentCheckpoint);
			return;	
		}
		
	}
	// 지정된 스폰 위치가 없을 경우 자동으로 PlayerStart 위치에서 스폰
	RestartPlayer(InController);
}

bool AHawnmunGameModeBase::IsBossEnemy(const AHawnmunEnemy* Enemy) const
{
	checkf(Enemy, TEXT("IsBossEnemy failed: Enemy is null."));

	return Enemy->ActorHasTag(TEXT("NineTailed")) ||
		   Enemy->ActorHasTag(TEXT("WildBoar"));
}
#pragma endregion

#pragma region Dungeon System
void AHawnmunGameModeBase::MergeDungeonLoot(AController* InController)
{
	if (!InController) return;

	AHawnmunPlayerState* PS = InController->GetPlayerState<AHawnmunPlayerState>();
	if (!PS) return;

	UInventoryComponent* InvComp = PS->GetInventoryComponent();
	if (!InvComp) return;

	if (auto* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		if (DungeonSubsystem->IsInDungeonSession())
		{
			const TMap<FGameplayTag, int32>& LootBag = DungeonSubsystem->GetSessionLootBag();
			for (const auto& Loot : LootBag)
			{
				InvComp->AddItem(Loot.Key, Loot.Value);
			}

			DungeonSubsystem->EndDungeonSession();
		}
	}
}

void AHawnmunGameModeBase::RegisterDungeonCleared()
{
	if (UHawnmunDungeonSubsystem* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		DungeonSubsystem->MarkDungeonCleared();
	}
}

void AHawnmunGameModeBase::CompleteDungeonClear(AController* InController)
{
	MergeDungeonLoot(InController);
	SaveProgressData();
}

void AHawnmunGameModeBase::AbandonDungeon()
{
	if (UHawnmunDungeonSubsystem* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		if (DungeonSubsystem->IsInDungeonSession())
		{
			DungeonSubsystem->EndDungeonSession();
		}
	}
	TravelToMap(TEXT("HideoutMap"));
}

float AHawnmunGameModeBase::GetDungeonClearTimeSeconds() const
{
	if (const UHawnmunDungeonSubsystem* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		return DungeonSubsystem->GetClearTimeSeconds();
	}

	return 0.f;
}

FText AHawnmunGameModeBase::GetDungeonClearTimeText() const
{
	const int32 TotalSeconds = FMath::FloorToInt(GetDungeonClearTimeSeconds());
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;

	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds));
}


#pragma endregion
