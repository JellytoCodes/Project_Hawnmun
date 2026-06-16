// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunEnemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/HawnmunAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "HawnmunGameplayTags.h"
#include "HUD/HawnmunHUD.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "BrainComponent.h"

#pragma region Core And Initialization
AHawnmunEnemy::AHawnmunEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	HawnmunAbilitySystemComponent = CreateDefaultSubobject<UHawnmunAbilitySystemComponent>("AbilitySystemComponent");
	HawnmunAttributeSet = CreateDefaultSubobject<UHawnmunAttributeSet>("AttributeSet");

	DropItemWidget = CreateDefaultSubobject<UWidgetComponent>("DropItemWidget");
	DropItemWidget->SetupAttachment(GetRootComponent());

	DorpItemInteract = CreateDefaultSubobject<USphereComponent>("DorpItemInteract");
	DorpItemInteract->SetupAttachment(GetRootComponent());
	DorpItemInteract->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DorpItemInteract->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginDropItemInteract);
	DorpItemInteract->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndDropItemInteract);
}

void AHawnmunEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}

void AHawnmunEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	HawnmunAIController = Cast<AHawnmunAIController>(NewController);
	if (BehaviorTree && HawnmunAIController)
	{
		HawnmunAIController->RunBehaviorTree(BehaviorTree);

		if (UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsFloat(TEXT("DefaultMaxWalkSpeed"), GetCharacterMovement()->MaxWalkSpeed);
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
		}

		if (!bIsEnemyActive)
		{
			HawnmunAIController->SetPerceptionEnabled(false);

			if (UBrainComponent* BrainComponent = HawnmunAIController->GetBrainComponent())
			{
				BrainComponent->StopLogic(TEXT("StartInactive"));
			}
		}
	}
	AddCharacterAbilities();
}

void AHawnmunEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitialSpawnTransform = GetActorTransform();

	if (DropItemWidget)
	{
		DropItemWidget->SetVisibility(false);
		UUserWidget* WidgetObj = DropItemWidget->GetUserWidgetObject();
		if (WidgetObj)
		{
			UTextBlock* MessageText = Cast<UTextBlock>(WidgetObj->GetWidgetFromName(TEXT("MessageText")));
			if (MessageText)
			{
				MessageText->SetText(FText::FromString(TEXT("아이템 줍기 (F)")));
			}
		}
	}
}
#pragma endregion

#pragma region Combat And State Management
void AHawnmunEnemy::SetSpawnedActorByBlackBoardKey(AActor* spawnedActor)
{
	if (UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject("SpawnedActor", spawnedActor);
	}
}

void AHawnmunEnemy::SetEnemyActive(bool bActive, AActor* InActor)
{
	bIsEnemyActive = bActive;

	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);

	GetCapsuleComponent()->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	DorpItemInteract->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DropItemWidget->SetVisibility(false);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	checkf(MovementComponent, TEXT("SetEnemyActive failed: MovementComponent is null on %s."), *GetNameSafe(this));
	MovementComponent->StopMovementImmediately();
	bActive ? MovementComponent->SetMovementMode(MOVE_Walking) : MovementComponent->DisableMovement();

	checkf(HawnmunAIController, TEXT("SetEnemyActive failed: HawnmunAIController is null on %s."), *GetNameSafe(this));
	HawnmunAIController->StopMovement();
	HawnmunAIController->SetPerceptionEnabled(bActive);

	UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent();
	checkf(BlackboardComponent, TEXT("SetEnemyActive failed: BlackboardComponent is null on %s."), *GetNameSafe(this));
	BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), bActive ? InActor : nullptr);

	if (UBrainComponent* BrainComponent = HawnmunAIController->GetBrainComponent())
	{
		bActive ? BrainComponent->RestartLogic() : BrainComponent->StopLogic(TEXT("EnemyInactive"));
	}

	if (bActive && InActor)
	{
		if (AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->AddAggroEnemy(this);
		}
	}
}

void AHawnmunEnemy::ResetToSpawnPoint()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("ResetToSpawnPoint failed: AbilitySystemComponent is null on %s."), *GetNameSafe(this));
	ASC->CancelAllAbilities();

	GetWorldTimerManager().ClearAllTimersForObject(this);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	checkf(AnimInstance, TEXT("ResetToSpawnPoint failed: AnimInstance is null on %s."), *GetNameSafe(this));
	AnimInstance->Montage_Stop(0.f);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
	    AIController->StopMovement();

	    if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
	    {
	        BrainComponent->StopLogic(TEXT("ResetToSpawnPoint"));
	    }
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	checkf(MovementComponent, TEXT("ResetToSpawnPoint failed: CharacterMovementComponent is null on %s."), *GetNameSafe(this));
	MovementComponent->StopMovementImmediately();

	const float MaxHealth = HawnmunAbilitySystemComponent->GetNumericAttribute(UHawnmunAttributeSet::GetMaxHealthAttribute());
	HawnmunAbilitySystemComponent->SetNumericAttributeBase(UHawnmunAttributeSet::GetHealthAttribute(), MaxHealth);

	SetActorTransform(InitialSpawnTransform, false, nullptr, ETeleportType::ResetPhysics);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
		{
			BTComp->RestartTree();
			if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
			{
				BlackboardComponent->SetValueAsObject("TargetActor", nullptr);
			}
		}
	}
}

int32 AHawnmunEnemy::GetCharacterLevel_Implementation()
{
	return 1;
}

void AHawnmunEnemy::Die()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	checkf(ASC, TEXT("Die: AbilitySystemComponent is null"));

	if (ASC->HasMatchingGameplayTag(HawnmunGameplayTags::State_Dead)) return;
	

	ASC->SetLooseGameplayTagCount(HawnmunGameplayTags::State_Dead, 1);

	bIsEnemyActive = false;
	ASC->CancelAllAbilities();

	checkf(HawnmunAIController, TEXT("Die: AIController is null"));

	HawnmunAIController->StopMovement();
	HawnmunAIController->SetPerceptionEnabled(false);

	UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent();

	checkf(BlackboardComponent, TEXT("Die: BlackboardComponent is null"));
	BlackboardComponent->ClearValue(TEXT("TargetActor"));

	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DorpItemInteract->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (GameMode)
	{
		GameMode->RemoveAggroEnemy(this);
	}

	HideBossOverlay();
	DieEvent();
}

bool AHawnmunEnemy::TryPhaseTransition()
{
	return false;
}

void AHawnmunEnemy::SetAggroTarget_Implementation(AActor* TargetActor)
{
	if (!TargetActor || !HawnmunAIController || !bIsEnemyActive || IsDead_Implementation()) return;

	UBlackboardComponent* BlackboardComponent = HawnmunAIController->GetBlackboardComponent();
	if (!BlackboardComponent) return;

	if (BlackboardComponent->GetValueAsObject(TEXT("TargetActor"))) return;

	BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), TargetActor);

	if (AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->AddAggroEnemy(this);
	}
}

void AHawnmunEnemy::HideBossOverlay() const
{
	if (!ActorHasTag(FName("NineTailed")) && !ActorHasTag(FName("WildBoar"))) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	AHawnmunHUD* HUD = Cast<AHawnmunHUD>(PC->GetHUD());
	if (!HUD) return;

	UOverlayWidgetController* OverlayWC = HUD->GetOverlayWidgetController(FWidgetControllerParams());
	if (!OverlayWC) return;

	OverlayWC->ClearBossAttributes();
}
#pragma endregion

#pragma region Loot And Interaction
void AHawnmunEnemy::OnBeginDropItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsLooted && OtherActor->Implements<UPlayerInterface>())
	{
		if (DropItemWidget) DropItemWidget->SetVisibility(true);
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, true, EInteractableList::Loot, this);
	}
}

void AHawnmunEnemy::OnEndDropItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (DropItemWidget) DropItemWidget->SetVisibility(false);
		IPlayerInterface::Execute_SetIsInteractable(OtherActor, false, EInteractableList::None, nullptr);
	}
}

TMap<FGameplayTag, int32> AHawnmunEnemy::ExtractLoot()
{
	if (bIsLooted) return TMap<FGameplayTag, int32>();

	bIsLooted = true;
	if (DropItemWidget) DropItemWidget->SetVisibility(false);
	DorpItemInteract->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TMap<FGameplayTag, int32> FinalLoot;

	for (const auto& Pair : LootTable)
	{
		float RandomDice = FMath::FRandRange(0.0f, 100.0f);
		
		if (RandomDice <= Pair.Value.DropChance)
		{
			FinalLoot.Add(Pair.Key, Pair.Value.Amount);
		}
	}

	return FinalLoot;
}
#pragma endregion