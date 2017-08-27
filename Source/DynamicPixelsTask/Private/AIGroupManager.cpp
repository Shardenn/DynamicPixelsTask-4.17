// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGroupManager.h"
#include "EngineUtils.h"
#include "EnemyAIController.h"
#include "PickUp.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

// Sets default values
AAIGroupManager::AAIGroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIGroupManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeBotsArray();
	
	PlayerCharacter = AActor::GetWorld()->GetFirstPlayerController()->GetCharacter();
	PickupItem = FindPickupItem();

	WhichActorBotReached.AddDynamic(this, &AAIGroupManager::CheckReachedActor);
}

// Called every frame
void AAIGroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DistanceFromPlayerToPickup = Cast<AActor>(PlayerCharacter)->GetHorizontalDistanceTo(PickupItem); // Cast is needed, doesnt work otherwise

	if ((DistanceFromPlayerToPickup > MinDistanceToPlayer + TakeItemDistance + 10.f) && (!PickupItem->GetAttachParentActor()))
	{
		CurrentTarget = SetAllBotsRunToActor(PickupItem, PickupAcceptanceRadius);
	}
	
	if (CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current target is %s"), *CurrentTarget->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Current target is null"));
	}
}


APickUp* AAIGroupManager::FindPickupItem()
{
	for (TActorIterator<APickUp>ActorItr(AActor::GetWorld()); ActorItr; ++ActorItr)
	{
		//if (ActorItr->IsActorInitialized() && ActorItr->HasActorBegunPlay())
		return *ActorItr;
	}
	return NULL;
}

void AAIGroupManager::InitializeBotsArray()
{
	
	for (TActorIterator<AEnemyAIController>BotItr(AActor::GetWorld()); BotItr; ++BotItr)
	{
		if (BotItr->IsActorInitialized() && BotItr->HasActorBegunPlay())
		{
			BotControllers.Add(*BotItr);
			BotItr->Manager = this;
		}
	}
	return;
}

AActor* AAIGroupManager::SetAllBotsRunToActor(AActor * Target, float AcceptanceDistance)
{
	for (auto BotItr : BotControllers)
	{
		if (BotItr->GetMoveStatus() < EPathFollowingStatus::Moving)
		{
			BotItr->ClearFocus(EAIFocusPriority::Gameplay);
			BotItr->MoveToActor(Target, AcceptanceDistance);
		}
	}
	return Target;
}

void AAIGroupManager::SetAllBotsLookAtPlayer()
{
	for (auto BotItr : BotControllers)
	{
		BotItr->SetFocus(PlayerCharacter, EAIFocusPriority::Gameplay);
	}
}

void AAIGroupManager::StopAllBotsMovement()
{
	for (auto BotItr : BotControllers)
	{
		if ((BotItr) && (BotItr->GetMoveStatus() == EPathFollowingStatus::Moving))
			BotItr->StopMovement();
	}
}


void AAIGroupManager::CheckReachedActor(AActor * MovingBot)
{
	//StopAllBotsMovement();
	
	//========================================
	if ( (CurrentTarget == PickupItem) && (!PickupItem->GetAttachParentActor()) ) // If any bot reached ball and it doesnt have parent
	{	
		//UE_LOG(LogTemp, Warning, TEXT("Current target is Pickup, does NOT have parent"));
		AttachItemToActor(MovingBot);
		StopAllBotsMovement();
		CurrentTarget = SetAllBotsRunToActor(PlayerCharacter, MinDistanceToPlayer);	
	}
	else if ( (CurrentTarget == PlayerCharacter) && (!PlayerCharacter->GetController()->IsMoveInputIgnored()) ) // if any bot reached player
	{																									// and player can move
		//UE_LOG(LogTemp, Warning, TEXT("Current target is Player, does have input"));
		PlayerCharacter->GetController()->SetIgnoreMoveInput(true);								//(that means that this bot is first one)
		StopAllBotsMovement();
		CurrentTarget = SurroundPlayer();	
	}
	else if (!CurrentTarget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current target is NULL"));
		SetAllBotsLookAtPlayer();
		CheckSurrounding();
	}
}


void AAIGroupManager::AttachItemToActor(AActor* Bot)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is inside AttachItemToActor func"), *Bot->GetName());
	// Turn off all physics and velocity
	if (!PickupItem->GetAttachParentActor())
	{
		PickupItem->TurnPhysicsOn(false);
		PickupItem->AttachToActor(Bot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PickupItem->SetActorRelativeLocation(TakenItemPosition);
	}
}

void AAIGroupManager::DetachItemFromActor()
{
	PickupItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickupItem->TurnPhysicsOn(true);
}

void AAIGroupManager::CheckSurrounding()
{
	if (++BotsSurroundedPlayer == BotControllers.Num())
	{
		PickupItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PickupItem->TurnPhysicsOn(true);
		PlayerCharacter->GetController()->SetIgnoreMoveInput(false);
		BotsSurroundedPlayer = 0;
	}
}


AActor* AAIGroupManager::SurroundPlayer()
{
	int32 BotIndex = 0;
	for (auto BotItr : BotControllers)
	{
		if (BotItr->GetMoveStatus() < EPathFollowingStatus::Moving)
		{
			FVector CircleLocation = PlayerCharacter->GetActorLocation() + LocationAroundPlayer(BotIndex++);

			if (IsPositionReachable(BotItr->GetPawn()->GetActorLocation(), CircleLocation))
			{
				BotItr->MoveToLocation(CircleLocation);
			}
			else
			{
				BotItr->MoveToActor(PlayerCharacter, MinDistanceToPlayer);
			}
			//BotItr->SetFocus(PlayerCharacter, EAIFocusPriority::Gameplay);

		}
	}
	return NULL;
}

bool AAIGroupManager::IsPositionReachable(FVector StartPosition, FVector TargetPosition)
{
	UNavigationPath* NavPath = UNavigationSystem::FindPathToLocationSynchronously(GetWorld(), StartPosition, TargetPosition, NULL);

	if (!NavPath)
		return false;

	return (NavPath->IsValid());
}

FVector AAIGroupManager::LocationAroundPlayer(int32 BotIndex)
{
	AngleRad = FMath::DegreesToRadians(360.f / BotControllers.Num());
	float XCoord = 0.f;
	float YCoord = 0.f;

	XCoord = MinDistanceToPlayer * FMath::Cos(BotIndex * AngleRad);
	YCoord = MinDistanceToPlayer * FMath::Sin(BotIndex * AngleRad);

	return FVector(XCoord, YCoord, 0.f);
}
