// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailed.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "HawnmunGameplayTags.h"
#include "Actors/FoxFireDropActor.h"
#include "Actors/PortalBreathActor.h"
#include "Actors/PortalBiteActor.h"
#include "NiagaraComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Characters/HawnmunNineTailedIllusion.h"
#include "Algo/RandomShuffle.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "ChaosClothAsset/ClothComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "HUD/HawnmunHUD.h"
#include "HUD/WidgetController/CinematicWidgetController.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

#pragma region Core And Initialization
AHawnmunNineTailed::AHawnmunNineTailed()
{
	PrimaryActorTick.bCanEverTick = false;

	CharmIllusionEffect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	CharmIllusionEffect->SetAutoActivate(false);
	CharmIllusionEffect->SetupAttachment(GetMesh());

	CachedCenterLocation = FVector::ZeroVector;

	BottomInnerClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_BottomInnerClothes"));
	BottomInnerClothes->SetupAttachment(GetMesh());

	BottomOuterClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_BottomOuterClothes"));
	BottomOuterClothes->SetupAttachment(GetMesh());

	RobeClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_Robe"));
	RobeClothes->SetupAttachment(GetMesh());

	TopClothes = CreateDefaultSubobject<UChaosClothComponent>(TEXT("CC_TopClothes"));
	TopClothes->SetupAttachment(GetMesh());

	EarEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EarEffect"));
	EarEffect->SetupAttachment(GetMesh(), TEXT("EarSocket"));
	EarEffect->SetAutoActivate(false);

	TailEffect01 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_01"));
	TailEffect01->SetupAttachment(GetMesh(), TEXT("Tail_01"));
	TailEffect01->SetAutoActivate(false);

	TailEffect02 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_02"));
	TailEffect02->SetupAttachment(GetMesh(), TEXT("Tail_02"));
	TailEffect02->SetAutoActivate(false);

	TailEffect03 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_03"));
	TailEffect03->SetupAttachment(GetMesh(), TEXT("Tail_03"));
	TailEffect03->SetAutoActivate(false);

	TailEffect04 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_04"));
	TailEffect04->SetupAttachment(GetMesh(), TEXT("Tail_04"));
	TailEffect04->SetAutoActivate(false);

	TailEffect05 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_05"));
	TailEffect05->SetupAttachment(GetMesh(), TEXT("Tail_05"));
	TailEffect05->SetAutoActivate(false);

	TailEffect06 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect_06"));
	TailEffect06->SetupAttachment(GetMesh(), TEXT("Tail_06"));
	TailEffect06->SetAutoActivate(false);
}

void AHawnmunNineTailed::BeginPlay()
{
	Super::BeginPlay();

	ValidateRequiredAssets();

	CreatePortalBreath();
	CreateFoxFireDrops();
	CreateNineTailedIllusion();
	CreatePortalBites();
	
	CachedCenterLocation = GetActorLocation();
	GenerateCirclePoints(800.f);

	// 명시적으로 꼬리, 귀, 로브 상태 초기화
	ApplyPhase1VisualState();
}

void AHawnmunNineTailed::ValidateRequiredAssets() const
{
	checkf(!SpawnFoxFireDropClass.IsNull(), TEXT("NineTailed setup failed: SpawnFoxFireDropClass is not assigned."));

    checkf(!SpawnPortalBreathClass.IsNull(), TEXT("NineTailed setup failed: SpawnPortalBreathClass is not assigned."));

    checkf(!SpawnPortalBiteClass.IsNull(), TEXT("NineTailed setup failed: SpawnPortalBiteClass is not assigned."));

    checkf(!SpawnNineTailedIllusionClass.IsNull(), TEXT("NineTailed setup failed: SpawnNineTailedIllusionClass is not assigned."));

	checkf(Phase1RobeAsset, TEXT("ApplyPhase1VisualState failed: Phase1RobeAsset is not assigned."));
	checkf(Phase2RobeAsset, TEXT("ApplyPhase2VisualState failed: Phase2RobeAsset is not assigned."));
}
#pragma endregion

#pragma region Combat Interface And Phase
int32 AHawnmunNineTailed::GetCharacterLevel_Implementation()
{
	return 1;
}

void AHawnmunNineTailed::Die()
{
	StopNineTailedCombatLogic(TEXT("NineTailedDied"));
	HideIllusionsForDeath();

	Super::Die();

	if (AHawnmunPlayer* Player = Cast<AHawnmunPlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->ProcessLooting(this);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD());
		checkf(HUD, TEXT("Die failed: HUD is null."));

		HUD->ShowCinematicWidget();
		HUD->GetCinematicSlotWidgetController(FWidgetControllerParams())->RequestPlayCinematic(HawnmunGameplayTags::Cinematic_NineTailed_Defeat, this);
	}
}

bool AHawnmunNineTailed::TryPhaseTransition()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("TryPhaseTransition failed: ASC is null on %s."), *GetNameSafe(this));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Enemy_State_Boss_Phase2))
	{
		return false;
	}

	StartPhase2Transition();
	return true;
}

void AHawnmunNineTailed::ResumeEnemyAI_Implementation()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("ResumeEnemyAI failed: ASC is null on %s."), *GetNameSafe(this));

	AHawnmunGameModeBase* GM = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));
	checkf(GM, TEXT("ResumeEnemyAI failed: GameMode is null."));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Dead))
	{
		HandleDungeonCleared();
		GM->ChangeBGMByTag(HawnmunGameplayTags::BGM_CombatField_Default, 0.5f, 0.f);
		return;
	}

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Enemy_State_Boss_Phase2))
	{
		EnterPhase2();
		GM->ChangeBGMByTag(HawnmunGameplayTags::BGM_NineTailed_Phase2, 0.5f, 0.f);
		return;
	}

	GM->ChangeBGMByTag(HawnmunGameplayTags::BGM_NineTailed_Phase1, 0.5f, 0.f);
}

void AHawnmunNineTailed::EnterPhase2()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	RefillPhase2Health();

	GenerateCirclePoints(800.f);
	ApplyPhase2VisualState();

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->ApplyPhase2VisualState();
		}
	}

	if (Phase2BehaviorTree)
	{
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->RunBehaviorTree(Phase2BehaviorTree);
		}
	}
}

void AHawnmunNineTailed::StartPhase2Transition()
{
	StopNineTailedCombatLogic(TEXT("NineTailedPhaseTransition"));
	ForceEndIllusionPatternForPhaseTransition();

	MovePlayerToPhase2TransitionPoint();
	SetDungeonPhaseDataLayerState(true);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddLooseGameplayTag(HawnmunGameplayTags::Enemy_State_Boss_Phase2);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD());
		checkf(HUD, TEXT("StartPhase2Transition failed: HUD is null."));

		HUD->ShowCinematicWidget();
		HUD->GetCinematicSlotWidgetController(FWidgetControllerParams())->RequestPlayCinematic(HawnmunGameplayTags::Cinematic_NineTailed_Phase2, this);
	}
}

void AHawnmunNineTailed::StopNineTailedCombatLogic(const FString& Reason)
{
	ClearNineTailedPatternTimers();
	CancelPortalBreathPatterns();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->CancelAllAbilities();
	}

	if (UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		AnimInstance->Montage_Stop(0.f);
	}

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();

		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(Reason);
		}

		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
		}
	}

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->StopMovementImmediately();
	}
}

void AHawnmunNineTailed::ClearNineTailedPatternTimers()
{
	for (FTimerHandle& Handle : FoxFireRandDropTimerHandles)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	FoxFireRandDropTimerHandles.Reset();

	for (FTimerHandle& Handle : PortalBiteTimerHandles)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	PortalBiteTimerHandles.Reset();

	GetWorldTimerManager().ClearTimer(IllusionSequenceTimerHandle);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AHawnmunNineTailed::RefillPhase1Health()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("RefillPhase1Health failed: ASC is null on %s."), *GetNameSafe(this));

	ASC->SetNumericAttributeBase(UHawnmunAttributeSet::GetMaxHealthAttribute(), Phase1Health);
	ASC->SetNumericAttributeBase(UHawnmunAttributeSet::GetHealthAttribute(), Phase1Health);
}

void AHawnmunNineTailed::RefillPhase2Health()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("RefillPhase2Health failed: ASC is null on %s."), *GetNameSafe(this));

	const FGameplayAttribute MaxHealthAttribute = UHawnmunAttributeSet::GetMaxHealthAttribute();
	const FGameplayAttribute HealthAttribute = UHawnmunAttributeSet::GetHealthAttribute();

	ASC->SetNumericAttributeBase(MaxHealthAttribute, Phase2Health);

	const float AppliedMaxHealth = ASC->GetNumericAttribute(MaxHealthAttribute);
	ASC->SetNumericAttributeBase(HealthAttribute, AppliedMaxHealth);
}

void AHawnmunNineTailed::ClearPhase2State()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::Enemy_State_Boss_Phase2, 0);
	}
}

void AHawnmunNineTailed::HandleDungeonCleared()
{
	if (AHawnmunGameModeBase* GM = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->RegisterDungeonCleared();
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD()))
		{
			HUD->ShowMissionClearWidget();
		}
	}
}

void AHawnmunNineTailed::ForceCancelPlayerTargetLock()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerPawn);
	if (!PlayerASC) return;

	FGameplayTagContainer TargetLockTags;
	TargetLockTags.AddTag(HawnmunGameplayTags::Ability_TargetLock);

	PlayerASC->CancelAbilities(&TargetLockTags);
	PlayerASC->SetLooseGameplayTagCount(HawnmunGameplayTags::State_TargetLock, 0);
}

void AHawnmunNineTailed::MovePlayerToPhase2TransitionPoint() const
{
	if (!Phase2PlayerTransitionPoint) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	const FVector TargetLocation = Phase2PlayerTransitionPoint->GetActorLocation();
	const FRotator TargetRotation = Phase2PlayerTransitionPoint->GetActorRotation();

	PlayerPawn->SetActorLocationAndRotation(
		TargetLocation,
		TargetRotation,
		false,
		nullptr,
		ETeleportType::ResetPhysics
	);
}

void AHawnmunNineTailed::ResetToSpawnPoint()
{
	StopNineTailedCombatLogic(TEXT("NineTailedReset"));

	Super::ResetToSpawnPoint();

	SetDungeonPhaseDataLayerState(false);

	ClearPhase2State();
	RefillPhase1Health();

	ApplyPhase1VisualState();

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->ApplyPhase1VisualState();
		}
	}

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->RunBehaviorTree(BehaviorTree);
	}
}

ECombatDamageScalingStat AHawnmunNineTailed::GetDamageScalingStat_Implementation() const
{
	return ECombatDamageScalingStat::Intelligence;
}
#pragma endregion

#pragma region Pattern Fox Fire
void AHawnmunNineTailed::ActiveFoxFireCardinalDrop(const FDamageEffectParams& DamageEffectParams)
{
	const FVector CenterLoc = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();

	TArray<FVector> Offsets = {
		Forward * FoxFireDropDistance,
		-Forward * FoxFireDropDistance,
		Right * FoxFireDropDistance,
		-Right * FoxFireDropDistance
	};

	for (int32 i = 0; i < Offsets.Num(); ++i)
	{
		FoxFireDrops[i]->SetActorLocation(CenterLoc + Offsets[i]);
		FoxFireDrops[i]->ActiveFoxFireDrop(DamageEffectParams);
	}
}

void AHawnmunNineTailed::ActiveFoxFireDiagonalDrop(const FDamageEffectParams& DamageEffectParams)
{
	const FVector CenterLoc = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();

	TArray<FVector> Offsets = {
		(Forward + Right).GetSafeNormal() * FoxFireDropDistance,
		(-Forward - Right).GetSafeNormal() * FoxFireDropDistance,
		(Forward - Right).GetSafeNormal() * FoxFireDropDistance,
		(-Forward + Right).GetSafeNormal() * FoxFireDropDistance
	};

	for (int32 i = 0; i < Offsets.Num(); ++i)
	{
		FoxFireDrops[i]->SetActorLocation(CenterLoc + Offsets[i]);
		FoxFireDrops[i]->ActiveFoxFireDrop(DamageEffectParams);
	}
}

void AHawnmunNineTailed::ActiveFoxFireRandDrop(const FDamageEffectParams& DamageEffectParams)
{
	const FVector CenterLoc = GetActorLocation();

	for (FTimerHandle& Handle : FoxFireRandDropTimerHandles)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	FoxFireRandDropTimerHandles.Reset();

	for (int32 i = 0; i < FoxFireDrops.Num(); ++i)
	{
		if (AFoxFireDropActor* DropActor = FoxFireDrops[i])
		{
			FVector2D RandPoint = FMath::RandPointInCircle(FoxFireRandDropRadius);
			FVector TargetLocation = CenterLoc + FVector(RandPoint.X, RandPoint.Y, 0.f);

			float DelayTime = i * FoxFireRandDropInterval;

			if (DelayTime <= 0.f)
			{
				ExecuteRandDrop(DropActor, TargetLocation, DamageEffectParams);
			}
			else
			{
				FTimerDelegate TimerDel;
				TimerDel.BindUObject(this, &ThisClass::ExecuteRandDrop, DropActor, TargetLocation, DamageEffectParams);

				FTimerHandle NewHandle;
				GetWorldTimerManager().SetTimer(NewHandle, TimerDel, DelayTime, false);

				FoxFireRandDropTimerHandles.Add(NewHandle);
			}
		}
	}
}

void AHawnmunNineTailed::ActiveRandomSingleFoxFirePattern(const FDamageEffectParams& DamageEffectParams)
{
	FMath::RandBool() ? ActiveFoxFireCardinalDrop(DamageEffectParams) : ActiveFoxFireDiagonalDrop(DamageEffectParams);
}

void AHawnmunNineTailed::ActiveRandomSequenceFoxFirePattern(const FDamageEffectParams& DamageEffectParams)
{
	bool bIsCardinalFirst = FMath::RandBool();

	if (bIsCardinalFirst) ActiveFoxFireCardinalDrop(DamageEffectParams);
	else ActiveFoxFireDiagonalDrop(DamageEffectParams);

	FTimerHandle SequenceTimer;
	FTimerDelegate SequenceDelegate;
	SequenceDelegate.BindUObject(this, &ThisClass::ActiveRandomSequenceFoxFirePattern_Internal, bIsCardinalFirst, DamageEffectParams);

	GetWorld()->GetTimerManager().SetTimer(SequenceTimer, SequenceDelegate, 1.5f, false);
}

void AHawnmunNineTailed::CreateFoxFireDrops()
{
	UClass* Cls = SpawnFoxFireDropClass.LoadSynchronous();
    checkf(Cls, TEXT("NineTailed setup failed: failed to load SpawnFoxFireDropClass."));
	
	for (int i = 0; i < 8; i++)
	{
		FTransform SpawnTransform = GetActorTransform();
		if (AFoxFireDropActor* SpawnedFoxFireDrop = GetWorld()->SpawnActorDeferred<AFoxFireDropActor>(Cls, SpawnTransform))
		{
			SpawnedFoxFireDrop->AddIgnoreActor(this);
			SpawnedFoxFireDrop->FinishSpawning(SpawnTransform);

			FoxFireDrops.Add(SpawnedFoxFireDrop);
		}
	}
}

void AHawnmunNineTailed::ActiveRandomSequenceFoxFirePattern_Internal(bool bIsCardinalFirst, FDamageEffectParams DamageEffectParams)
{
	if (bIsCardinalFirst) ActiveFoxFireDiagonalDrop(DamageEffectParams);
	else ActiveFoxFireCardinalDrop(DamageEffectParams);
}

void AHawnmunNineTailed::ExecuteRandDrop(AFoxFireDropActor* DropActor, const FVector SpawnLocation, const FDamageEffectParams DamageParams)
{
	DropActor->SetActorLocation(SpawnLocation);
	DropActor->ActiveFoxFireDrop(DamageParams);
}
#pragma endregion

#pragma region Pattern Portal Breath
void AHawnmunNineTailed::ActivePortalBreath(int32 Amount, const FDamageEffectParams& DamageEffectParams)
{
	if (Amount == 2 && FieldPortalBreath.Num() % 2 == 0 && FieldPortalBreath.Num() >= 4)
	{
		int32 TotalPortals = FieldPortalBreath.Num();
		int32 HalfSize = TotalPortals / 2;

		int32 FirstIndex = FMath::RandRange(0, TotalPortals - 1);
		int32 OppositeIndex = (FirstIndex + HalfSize) % TotalPortals;

		int32 SecondIndex;
		do
		{
			SecondIndex = FMath::RandRange(0, TotalPortals - 1);
		} while (SecondIndex == FirstIndex || SecondIndex == OppositeIndex);

		FieldPortalBreath[FirstIndex]->ActivePortalBreath(DamageEffectParams);
		FieldPortalBreath[SecondIndex]->ActivePortalBreath(DamageEffectParams);
	}
	else
	{
		TArray<APortalBreathActor*> ShuffledPortals = FieldPortalBreath;
		Algo::RandomShuffle(ShuffledPortals);

		int32 FireCount = FMath::Min(Amount, ShuffledPortals.Num());

		for (int32 i = 0; i < FireCount; i++)
		{
			ShuffledPortals[i]->ActivePortalBreath(DamageEffectParams);
		}
	}
}

void AHawnmunNineTailed::CreatePortalBreath()
{
	const FVector CenterLoc = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const float AngleStep = 360.0f / 8.0f;

	UClass* Cls = SpawnPortalBreathClass.LoadSynchronous();
	checkf(Cls, TEXT("NineTailed setup failed: failed to load SpawnPortalBreathClass."));

	for (int32 i = 0; i < 8; i++)
	{
		FTransform SpawnTransform = GetActorTransform();
		
		FVector Direction = Forward.RotateAngleAxis(i * AngleStep, FVector::UpVector);
		FVector SpawnLocation = CenterLoc + (Direction.GetSafeNormal() * PortalSpawnDistance);
		SpawnTransform.SetLocation(SpawnLocation);

		FVector DirectionToCenter = (CenterLoc - SpawnLocation).GetSafeNormal();
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToCenter).Rotator();
		LookAtRotation.Pitch = 0.f;
		LookAtRotation.Roll = 0.f;
		SpawnTransform.SetRotation(LookAtRotation.Quaternion());

		if (APortalBreathActor* SpawnedPortalBreath = GetWorld()->SpawnActorDeferred<APortalBreathActor>(Cls, SpawnTransform))
		{
			SpawnedPortalBreath->AddIgnoreActor(this);
			SpawnedPortalBreath->FinishSpawning(SpawnTransform);
			FieldPortalBreath.Add(SpawnedPortalBreath);
		}
	}
	
}

void AHawnmunNineTailed::CancelPortalBreathPatterns()
{
	for (APortalBreathActor* PortalBreath : FieldPortalBreath)
	{
		PortalBreath->CancelPortalBreath();
	}
}
#pragma endregion

#pragma region Pattern Portal Bite
void AHawnmunNineTailed::ActivePortalBitePattern(const FVector& TargetLocation, const FDamageEffectParams& DamageParams, int32 Amount, float Interval)
{
	for (FTimerHandle& Handle : PortalBiteTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
	PortalBiteTimerHandles.Reset();

	for (int32 i = 0; i < Amount; ++i)
	{
		float DelayTime = i * Interval;

		if (DelayTime <= 0.f)
		{
			ExecutePortalBite(TargetLocation, DamageParams);
		}
		else
		{
			FTimerDelegate TimerDel;
			TimerDel.BindUObject(this, &ThisClass::ExecutePortalBite, TargetLocation, DamageParams);
			FTimerHandle NewHandle;
			GetWorld()->GetTimerManager().SetTimer(NewHandle, TimerDel, DelayTime, false);

			PortalBiteTimerHandles.Add(NewHandle);
		}
	}
}

void AHawnmunNineTailed::ExecutePortalBite(FVector TargetLocation, FDamageEffectParams DamageParams)
{
	if (APortalBiteActor* TargetActor = PortalBitePool[CurrentPortalBiteIndex])
	{
		TargetActor->ActivatePortalBite(TargetLocation, DamageParams);
	}
	CurrentPortalBiteIndex = (CurrentPortalBiteIndex + 1) % PortalBitePool.Num();
}

void AHawnmunNineTailed::CreatePortalBites()
{
	UClass* Cls = SpawnPortalBiteClass.LoadSynchronous();
	checkf(Cls, TEXT("NineTailed setup failed: failed to load SpawnPortalBiteClass."));

	for (int32 i = 0; i < 3; i++)
	{
		FTransform SpawnTransform = GetActorTransform();
		if (APortalBiteActor* SpawnedActor = GetWorld()->SpawnActorDeferred<APortalBiteActor>(Cls, SpawnTransform))
		{
			SpawnedActor->SetInstigator(this);
			SpawnedActor->FinishSpawning(SpawnTransform);
			
			PortalBitePool.Add(SpawnedActor);
		}
	}
}
#pragma endregion

#pragma region Pattern Charm Illusion
void AHawnmunNineTailed::ActivateIllusionsEffect()
{
	for (auto& Illusion : NineTailedIllusion)
		Illusion->ActivateCharmIllusionEffect();
}

void AHawnmunNineTailed::DeactivateIllusionsEffect()
{
	for (auto& Illusion : NineTailedIllusion)
		Illusion->DeactivateCharmIllusionEffect();
}

void AHawnmunNineTailed::ShuffleNineTailedCharmIllusion()
{
	Algo::RandomShuffle(NineTailedIllusionLocation);

	int idx = 0;

	FVector RealLoc = NineTailedIllusionLocation[idx++];
	FRotator RealRot = (CachedCenterLocation - RealLoc).Rotation();
	SetActorLocationAndRotation(RealLoc, RealRot);

	for (auto& Illusion : NineTailedIllusion)
	{
		FVector IllusionLoc = NineTailedIllusionLocation[idx++];
		FRotator IllusionRot = (CachedCenterLocation - IllusionLoc).Rotation();
		IllusionRot.Pitch = 0.f;
		IllusionRot.Roll = 0.f;
		Illusion->SetActorLocationAndRotation(IllusionLoc, IllusionRot);
	}
}

void AHawnmunNineTailed::BackCenterLocation(const FDamageEffectParams& DamageEffectParams)
{
	SetActorLocation(CachedCenterLocation);

	for (auto& Illusion : NineTailedIllusion)
	{
		Illusion->CombatDamageEffectParams = DamageEffectParams;
		Illusion->CauseDamage();
	}
}

void AHawnmunNineTailed::ExecuteIllusionSequence(const FDamageEffectParams& DamageEffectParams)
{
	CachedIllusionDamageParams = DamageEffectParams;

	IllusionSequence_DisappearBody();
}

void AHawnmunNineTailed::SetCharmIllusionDissolveValue(float Value)
{
	const FName NiagaraAnimationName = TEXT("User_Animation");
	const FName MaterialAnimationName = TEXT("Animation");

	if (CharmIllusionEffect)
	{
		CharmIllusionEffect->SetVariableFloat(NiagaraAnimationName, Value);
	}

	if (GetMesh())
	{
		GetMesh()->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (BottomInnerClothes)
	{
		BottomInnerClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (BottomOuterClothes)
	{
		BottomOuterClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (RobeClothes)
	{
		RobeClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}

	if (TopClothes)
	{
		TopClothes->SetScalarParameterValueOnMaterials(MaterialAnimationName, Value);
	}
}

void AHawnmunNineTailed::CreateNineTailedIllusion()
{
	UClass* Cls = SpawnNineTailedIllusionClass.LoadSynchronous();
	checkf(Cls, TEXT("NineTailed setup failed: failed to load SpawnNineTailedIllusionClass."));

	for (int32 i = 0; i < 4; ++i)
	{
		FTransform SpawnTransform = GetActorTransform();

		if (AHawnmunNineTailedIllusion* SpawnedIllusion = GetWorld()->SpawnActorDeferred<AHawnmunNineTailedIllusion>(Cls, SpawnTransform))
		{
			SpawnedIllusion->FinishSpawning(SpawnTransform);
			SpawnedIllusion->ApplyPhase1VisualState();

			NineTailedIllusion.Add(SpawnedIllusion);
		}
	}
}

void AHawnmunNineTailed::GenerateCirclePoints(float Radius)
{
	NineTailedIllusionLocation.Empty();

	CachedCenterLocation = GetActorLocation();

	const float AngleStep = 360.0f / 5.0f;

	for (int32 i = 0; i < 5; ++i)
	{
		float CurrentAngle = i * AngleStep;
		float Rad = FMath::DegreesToRadians(CurrentAngle);

		FVector Offset(FMath::Cos(Rad) * Radius, FMath::Sin(Rad) * Radius, 0.f);
		NineTailedIllusionLocation.Add(CachedCenterLocation + Offset);
	}
}

void AHawnmunNineTailed::IllusionSequence_DisappearBody()
{
	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(false);
	ForceCancelPlayerTargetLock();

	ActivateCharmIllusionEffect();

	GetWorldTimerManager().SetTimer(
		IllusionSequenceTimerHandle,
		this,
		&AHawnmunNineTailed::IllusionSequence_AppearWithIllusions,
		5.f,
		false);
}

void AHawnmunNineTailed::IllusionSequence_AppearWithIllusions()
{
	ShuffleNineTailedCharmIllusion();

	DeactivateCharmIllusionEffect();
	DeactivateIllusionsEffect();

	SetCharacterCollisionEnabled(true);

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->SetCharacterCollisionEnabled(true);
		}
	}

	GetWorldTimerManager().SetTimer(
		IllusionSequenceTimerHandle,
		this,
		&AHawnmunNineTailed::IllusionSequence_DisappearAllAndDamage,
		7.f,
		false);
}

void AHawnmunNineTailed::IllusionSequence_DisappearAllAndDamage()
{
	ActivateCharmIllusionEffect();
	ActivateIllusionsEffect();

	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(false);

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->SetTailEarEffectsActive(false);
			Illusion->SetCharacterCollisionEnabled(false);
		}
	}

	GetWorldTimerManager().SetTimer(
		IllusionSequenceTimerHandle,
		this,
		&AHawnmunNineTailed::IllusionSequence_ReturnBody,
		4.f,
		false);
}

void AHawnmunNineTailed::IllusionSequence_ReturnBody()
{
	SetActorLocation(CachedCenterLocation);

	DeactivateCharmIllusionEffect();

	SetCharacterCollisionEnabled(true);
		for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->CombatDamageEffectParams = CachedIllusionDamageParams;
			Illusion->CauseDamage();
		}
	}

	GetWorldTimerManager().SetTimer(IllusionSequenceTimerHandle, [this]()
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			this,
			HawnmunGameplayTags::Event_NineTailed_IllusionEnd,
			FGameplayEventData());
	}, 4.f, false);
}
#pragma endregion

#pragma region Visual Phase
void AHawnmunNineTailed::SetRobeForPhase(bool bPhase2)
{
	RobeClothes->SetVisibility(true, true);
	RobeClothes->SetAsset(bPhase2 ? Phase2RobeAsset : Phase1RobeAsset);
	RobeClothes->ForceNextUpdateTeleportAndReset();
}

void AHawnmunNineTailed::SetTailEarEffectsActive(bool bActive)
{
	if (bActive && !IsPhase2Active())
	{
		return;
	}

	static const FName ColorParameterName(TEXT("User.Opacity"));
	constexpr float ActiveOpacityValue = 1.f;

	UNiagaraComponent* Effects[] =
	{
		EarEffect,
		TailEffect01,
		TailEffect02,
		TailEffect03,
		TailEffect04,
		TailEffect05,
		TailEffect06
	};

	for (UNiagaraComponent* Effect : Effects)
	{
		if (!Effect) continue;

		Effect->SetVariableFloat(ColorParameterName,bActive ? ActiveOpacityValue : 0.f);
		bActive ? Effect->Activate(true) : Effect->Deactivate();
	}
}

void AHawnmunNineTailed::SetCharacterCollisionEnabled(bool bVisible)
{
	SetActorEnableCollision(bVisible);
}

bool AHawnmunNineTailed::IsPhase2Active() const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	return ASC && ASC->HasMatchingGameplayTag(HawnmunGameplayTags::Enemy_State_Boss_Phase2);
}

void AHawnmunNineTailed::ApplyPhase1VisualState()
{
	SetRobeForPhase(false);
	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(true);
	SetCharmIllusionDissolveValue(CharmIllusionVisibleValue);
}

void AHawnmunNineTailed::ApplyPhase2VisualState()
{
	SetRobeForPhase(true);
	SetTailEarEffectsActive(true);
	SetCharacterCollisionEnabled(true);
	SetCharmIllusionDissolveValue(CharmIllusionVisibleValue);
}

void AHawnmunNineTailed::ForceEndIllusionPatternForPhaseTransition()
{
	GetWorldTimerManager().ClearTimer(IllusionSequenceTimerHandle);

	SetActorLocation(CachedCenterLocation);
	SetCharacterCollisionEnabled(true);
	SetTailEarEffectsActive(false);
	SetCharmIllusionDissolveValue(CharmIllusionVisibleValue);

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->SetCharacterCollisionEnabled(false);
			Illusion->SetTailEarEffectsActive(false);
			Illusion->SetCharmIllusionDissolveValue(CharmIllusionHiddenValue);
		}
	}

	ForceCancelPlayerTargetLock();
}

void AHawnmunNineTailed::HideIllusionsForDeath()
{
	GetWorldTimerManager().ClearTimer(IllusionSequenceTimerHandle);

	SetTailEarEffectsActive(false);
	SetCharacterCollisionEnabled(false);
	SetCharmIllusionDissolveValue(CharmIllusionVisibleValue);

	for (AHawnmunNineTailedIllusion* Illusion : NineTailedIllusion)
	{
		if (Illusion)
		{
			Illusion->SetActorHiddenInGame(true);
			Illusion->SetCharacterCollisionEnabled(false);
			Illusion->SetTailEarEffectsActive(false);
			Illusion->SetCharmIllusionDissolveValue(CharmIllusionHiddenValue);
		}
	}

	ForceCancelPlayerTargetLock();
}

void AHawnmunNineTailed::SetDungeonPhaseDataLayerState(bool bPhase2)
{
	if (!Phase1DataLayer || !Phase2DataLayer) return;

	UDataLayerManager* DataLayerManager = UDataLayerManager::GetDataLayerManager(GetWorld());
	if (!DataLayerManager) return;

	DataLayerManager->SetDataLayerRuntimeState(
		Phase1DataLayer,
		bPhase2 ? EDataLayerRuntimeState::Loaded : EDataLayerRuntimeState::Activated,
		true
	);

	DataLayerManager->SetDataLayerRuntimeState(
		Phase2DataLayer,
		bPhase2 ? EDataLayerRuntimeState::Activated : EDataLayerRuntimeState::Loaded,
		true
	);
}
#pragma endregion
