// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGroupManager.h"
#include "EngineUtils.h"
#include "PickUp.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h" // GetCharacterMovement()

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

	LastPlayerPosition = PlayerCharacter->GetActorLocation();
}

// Called every frame
void AAIGroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget)
	{
		LastPlayerPosition = PlayerCharacter->GetActorLocation();
	}

	DistanceFromPlayerToPickup = FVector::DistXY(LastPlayerPosition, PickupItem->GetActorLocation());

	if ((DistanceFromPlayerToPickup > MinDistanceToPlayer + TakeItemDistance + 50.f) && (!PickupItem->GetAttachParentActor()))
	{
		CurrentTarget = SetAllBotsRunToActor(PickupItem, PickupAcceptanceRadius);
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
		if ( (BotItr->GetMoveStatus() < EPathFollowingStatus::Moving) && ( BotItr->GetPawn()->GetDefaultHalfHeight() < Target->GetActorLocation().Z ) )
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
		if(BotItr)
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
	if ( (CurrentTarget == PickupItem) && (!PickupItem->GetAttachParentActor()) && (MovingBot->GetHorizontalDistanceTo(PickupItem) <= TakeItemDistance)) // If any bot reached ball and it doesnt have parent
	{	
		AttachItemToActor(MovingBot);
		StopAllBotsMovement();
		CurrentTarget = SetAllBotsRunToActor(PlayerCharacter, MinDistanceToPlayer / 2.f);	
	}
	else if ( (CurrentTarget == PlayerCharacter) && (!PlayerCharacter->GetController()->IsMoveInputIgnored()) ) 
	{
		TurnOffPlayerMovement();	
		StopAllBotsMovement();
		CurrentTarget = SurroundPlayer();	
	}
	else if (!CurrentTarget)
	{
		SetAllBotsLookAtPlayer();
		CheckSurrounding();
	}
}


void AAIGroupManager::AttachItemToActor(AActor* Bot)
{
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
	if (PickupItem->GetAttachParentActor())
	{
		BotsSurroundedPlayer++;
		if (BotsSurroundedPlayer == BotControllers.Num())
		{
			BotsSurroundedPlayer = 0;  // two lines? 
			DetachItemFromActor();
			PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			PlayerCharacter->GetController()->SetIgnoreMoveInput(false);
		}
	}
}


AActor* AAIGroupManager::SurroundPlayer()
{
	int32 BotIndex = 0;
	LastPlayerPosition = PlayerCharacter->GetActorLocation();

	for (auto BotItr : BotControllers)
	{
		if ((BotItr) && (BotItr->GetMoveStatus() < EPathFollowingStatus::Moving))
		{
			FVector CircleLocation = PlayerCharacter->GetActorLocation() + LocationAroundPlayer(BotIndex++);
			CircleLocation.Z = FMath::Min(CircleLocation.Z, BotItr->GetPawn()->GetDefaultHalfHeight());

			if (IsPositionReachable(BotItr->GetPawn()->GetActorLocation(), CircleLocation))
			{
				BotItr->MoveToLocation(CircleLocation, 1.0f);
			}
			else
			{
				UNavigationSystem* NavSystem = AActor::GetWorld()->GetNavigationSystem();
				if (NavSystem)
				{
					CircleLocation = NavSystem->GetRandomPointInNavigableRadius(AActor::GetWorld(), PlayerCharacter->GetActorLocation(), MinDistanceToPlayer * 2.f);
				}
			}
			BotItr->MoveToLocation(CircleLocation, 0.f);
			//BotItr->SetFocus(PlayerCharacter, EAIFocusPriority::Gameplay);

		}
	}
	return NULL;
}

void AAIGroupManager::TurnOffPlayerMovement()
{
	PlayerCharacter->GetController()->SetIgnoreMoveInput(true);
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); 
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);  // looks like shit, TODO think about another way
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
	float AngleRad = FMath::DegreesToRadians(360.f / BotControllers.Num());
	float XCoord = 0.f;
	float YCoord = 0.f;

	XCoord = MinDistanceToPlayer * FMath::Cos(BotIndex * AngleRad);
	YCoord = MinDistanceToPlayer * FMath::Sin(BotIndex * AngleRad);

	return FVector(XCoord, YCoord, 0.f);
}

void AAIGroupManager::EndGame()
{
	CurrentTarget = NULL;
	StopAllBotsMovement();
	
	if (PickupItem->GetAttachParentActor())
		DetachItemFromActor();

	for (auto BotItr : BotControllers)
	{
		BotItr->GetPawn()->Destroy();
		BotItr->Destroy();
	}
	Destroy();
}