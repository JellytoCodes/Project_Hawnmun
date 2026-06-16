// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunPlayer.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Characters/HawnmunEnemy.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "Camera/CameraComponent.h"
#include "Controllers/HawnmunPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/HawnmunGameInstance.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "GameMode/HawnmunPlayerState.h"
#include "GameMode/HawnmunSaveGame.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "HUD/HawnmunHUD.h"
#include "Kismet/GameplayStatics.h"

#pragma region Core And Initialization
AHawnmunPlayer::AHawnmunPlayer()
{
	SubWeapon = CreateDefaultSubobject<UStaticMeshComponent>("SubWeapon");
	SubWeapon->SetupAttachment(GetMesh(), "SubWeaponHandSocket");
	SubWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SubWeapon->SetGenerateOverlapEvents(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 350.f;
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 80.f);
	SpringArmComponent->bUsePawnControlRotation = true;

	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 1.0f;
	SpringArmComponent->CameraLagMaxDistance = 200.0f;

	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 7.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AHawnmunPlayer::InitAbilityActorInfo()
{
	auto* HawnmunPlayerState = GetPlayerState<AHawnmunPlayerState>();
	checkf(HawnmunPlayerState, TEXT("InitAbilityActorInfo failed: HawnmunPlayerState is null for %s."), *GetNameSafe(this));
	
	checkf(HawnmunPlayerState->GetAbilitySystemComponent(), TEXT("InitAbilityActorInfo failed: ASC is null on PlayerState %s."), *GetNameSafe(HawnmunPlayerState));
	checkf(HawnmunPlayerState->GetAttributeSet(), TEXT("InitAbilityActorInfo failed: AttributeSet is null on PlayerState %s."), *GetNameSafe(HawnmunPlayerState));

	HawnmunPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(HawnmunPlayerState, this);
	HawnmunAbilitySystemComponent = HawnmunPlayerState->GetAbilitySystemComponent();
	HawnmunAttributeSet = HawnmunPlayerState->GetAttributeSet();

	HawnmunAbilitySystemComponent->SetLooseGameplayTagCount(HawnmunGameplayTags::State_Dead, 0);

	if (!PlayerController.IsValid()) return;

	if (AHawnmunHUD* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController.Get()->GetHUD()))
		HawnmunHUD->InitializeWidgets(PlayerController.Get(), HawnmunAbilitySystemComponent, HawnmunAttributeSet, HawnmunPlayerState);
}

void AHawnmunPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (auto* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		if (DungeonSubsystem->IsInDungeonSession() && !DungeonSubsystem->bHasCheckpoint)
		{
			DungeonSubsystem->SaveCheckpoint(GetActorTransform());
		}
	}

	if (auto* HawnmunPlayerState = GetPlayerState<AHawnmunPlayerState>())
	{
		HawnmunPlayerState->OnEquippedWeaponChangedDelegate.AddUObject(this, &AHawnmunPlayer::OnWeaponStateChanged);
	}

	SubWeapon->SetVisibility(false);
}

void AHawnmunPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!PlayerController.IsValid())
		PlayerController = Cast<AHawnmunPlayerController>(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
	LoadProgress();
}

void AHawnmunPlayer::LoadProgress()
{
	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));
	checkf(
		GameMode,
		TEXT("LoadProgress failed: %s requires AHawnmunGameModeBase, but current GameMode is %s."),
		*GetNameSafe(this),
		*GetNameSafe(UGameplayStatics::GetGameMode(this))
	);

	AHawnmunPlayerState* PS = GetPlayerState<AHawnmunPlayerState>();
	checkf(PS, TEXT("LoadProgress failed: PlayerState is null for %s."), *GetNameSafe(this));

	checkf(HawnmunAbilitySystemComponent,
	TEXT("LoadProgress failed: ASC is not initialized for %s."), *GetNameSafe(this));

	UHawnmunSaveGame* SaveData = GameMode->GetProgressSaveData();
	checkf(SaveData, TEXT("LoadProgress failed: GetProgressSaveData returned null."));

	if (SaveData && !SaveData->bFirstTimeLoadIn)
	{
		PS->SetPlayerLevel(SaveData->PlayerLevel);
		PS->SetXP(SaveData->PlayerXP);
		PS->SetAttributePoints(SaveData->AttributePoints);
		PS->LoadInventoryAndEquipment(SaveData);

		UHawnmunFunctionLibrary::InitializeAttributesFromSaveData(this, HawnmunAbilitySystemComponent, SaveData);
	}
	else 
	{
		SaveData->bFirstTimeLoadIn = false;

		if (!StartUpData.IsNull())
		{
			if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
			{
				LoadedData->InitializeStartupItem(PS->GetInventoryComponent());

				PS->EquipItem(HawnmunGameplayTags::Item_Equip_Weapon_NormalSword);
				PS->EquipItem(HawnmunGameplayTags::Item_Consumable_Ammo_NormalAmmo);
			}
		}

		GameMode->SaveProgressData();

		if (UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(GetGameInstance()))
		{
			if (GI->GetCurrentSaveSlotIndex() != -1)
			{
				GameMode->SyncProgressToSlot(GI->GetCurrentSaveSlotName(), GI->GetCurrentSaveSlotIndex());
			}
		}
	}

	// 게임 시작 or 던전 입장 등 항상 플레이어는 메인 무기 장착을 기본으로 할 수 있도록 함
	PS->EquippedWeaponChanged(true);

	if (HawnmunAbilitySystemComponent)
			HawnmunAbilitySystemComponent->AddLooseGameplayTag(HawnmunGameplayTags::Event_Equip_HasMainWeapon);

	if (StaminaRegen && HawnmunAbilitySystemComponent)
	{
		const UGameplayEffect* staminaRegen = StaminaRegen.GetDefaultObject();
		HawnmunAbilitySystemComponent->ApplyGameplayEffectToSelf(staminaRegen, 1, HawnmunAbilitySystemComponent->MakeEffectContext());
	}
}
#pragma endregion

#pragma region Player State And Actions
void AHawnmunPlayer::SetWeaponVisibility(bool bState)
{
	Weapon->SetVisibility(bState);
}

void AHawnmunPlayer::SetInLobby(const bool InState)
{
	if (PlayerController == nullptr) return;
	PlayerController->SetInLobby(InState);
}

void AHawnmunPlayer::ProcessLooting(AActor* TargetActor)
{
	if (!TargetActor) return;

	if (AHawnmunEnemy* Enemy = Cast<AHawnmunEnemy>(TargetActor))
	{
		TMap<FGameplayTag, int32> LootedItems = Enemy->ExtractLoot();

		if (LootedItems.IsEmpty()) return;
		auto* DungeonSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UHawnmunDungeonSubsystem>();
		
		if (DungeonSubsystem && DungeonSubsystem->IsInDungeonSession())
		{
			for (const auto& Loot : LootedItems)
			{
				DungeonSubsystem->RecordLootItem(Loot.Key, Loot.Value);
			}
		}
		else
		{
			if (AHawnmunPlayerState* PS = GetPlayerState<AHawnmunPlayerState>())
			{
				if (UInventoryComponent* InvComp = PS->GetInventoryComponent())
				{
					for (const auto& Loot : LootedItems)
					{
						InvComp->AddItem(Loot.Key, Loot.Value);
					}
				}
			}
		}
		SetIsInteractable_Implementation(false, EInteractableList::None, nullptr);
	}
}

void AHawnmunPlayer::OnWeaponStateChanged(const bool bIsMainWeapon)
{
	if (bIsMainWeapon)
	{
		Weapon->SetVisibility(true);
		SubWeapon->SetVisibility(false);
	}
	else
	{
		Weapon->SetVisibility(false);
		SubWeapon->SetVisibility(true);
	}
}
#pragma endregion

#pragma region Components And Data
UInventoryComponent* AHawnmunPlayer::GetInventoryComponent()
{
	if (AHawnmunPlayerState* PS = GetPlayerState<AHawnmunPlayerState>())
		return PS->GetInventoryComponent();

	return nullptr;
}

UEquipmentComponent* AHawnmunPlayer::GetEquipmentComponent()
{
	if (AHawnmunPlayerState* PS = GetPlayerState<AHawnmunPlayerState>())
		return PS->GetEquipmentComponent();

	return nullptr;
}
#pragma endregion

#pragma region Combat Interface
int32 AHawnmunPlayer::GetCharacterLevel_Implementation()
{
	if (AHawnmunPlayerState* PS = GetPlayerState<AHawnmunPlayerState>())
		return PS->GetPlayerLevel();

	return 1;
}

FVector AHawnmunPlayer::GetProjectileSpawnSocketLocation_Implementation(const FName SocketName)
{
	FName MuzzleSocketName = FName("MuzzleSocket");
	return SubWeapon->GetSocketLocation(MuzzleSocketName);
}

void AHawnmunPlayer::Die()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::State_Dead, 1);

		ASC->CancelAllAbilities();
		ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::State_TargetLock, 0);
	}

	if (AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->ResetAllAggroEnemies();
	}

	DieEvent();
}
#pragma endregion

#pragma region Player Interface
void AHawnmunPlayer::EnableMappingContext_Implementation()
{
	PlayerController->EnableDefaultMappingContext();

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController->GetHUD()))
		HawnmunHUD->ShowOverlay();
}

void AHawnmunPlayer::DisableMappingContext_Implementation()
{
	PlayerController->DisableDefaultMappingContext();

	if (auto* HawnmunHUD = Cast<AHawnmunHUD>(PlayerController->GetHUD()))
		HawnmunHUD->HideOverlay();
}

AHawnmunPlayerState* AHawnmunPlayer::GetHawnmunPS_Implementation()
{
	return GetPlayerState<AHawnmunPlayerState>();
}

void AHawnmunPlayer::SetIsInteractable_Implementation(const bool InIsInteractable, const EInteractableList InList, AActor* TargetActor)
{
	if (PlayerController.IsValid())
	{
		PlayerController->SetIsInteractable(InIsInteractable, InList, TargetActor);
	}
}

void AHawnmunPlayer::ShowDungeonMap_Implementation() const
{
	if (PlayerController.IsValid())
	{
		PlayerController->DungeonMapWidgetSwitching();
	}
}

void AHawnmunPlayer::HideDungeonMap_Implementation() const
{
	if (PlayerController.IsValid())
	{
		PlayerController->CloseCurrentWidget();
	}
}
#pragma endregion