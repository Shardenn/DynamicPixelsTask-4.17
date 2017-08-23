// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyAIController.h"
#include "EngineUtils.h" // For actor iterator functions
#include "PickUp.h"
#include "FPCharacter.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PickupItem = SetPickup();
	PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	PickupItem = SetPickup();

	MaxDistFromPlayerToPickUp = MaxDistToPlayer + ItemTakeRadius + 30.f;

	FPCharacter = Cast<AFPCharacter>(AActor::GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (FPCharacter)
	{
		UniqueBotIndex = FPCharacter->GiveBotUniqueNumber();
		UE_LOG(LogTemp, Warning, TEXT("%s has number %d"), *GetPawn()->GetName(), UniqueBotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No FPCharacter pointer in AI Controller."));
	}

	
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// TODO find a way for bots to evade movable obstacles

	//============================================// Main bot logic //============================================// 
	if (GetDistanceFromPlayerToPickup() > MaxDistFromPlayerToPickUp) // If ball is far from player
	{
		PlayerCharacter->GetController()->SetIgnoreMoveInput(false);

		if ((PickupItem->GetAttachParentActor()) &&
			(PickupItem->GetAttachParentActor()->GetClass()->IsChildOf(AEnemyAI::StaticClass())))  // If a bot already carrying the pick up
		{
			SurroundPlayer();
			// Look at player when bot is running to him
			SetFocus(Cast<AActor>(PlayerCharacter), EAIFocusPriority::Gameplay);
		}
		else // A parent of pick up is NOT a bot
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			// Run for a ball
			MoveToActor(PickupItem, ItemTakeRadius - 0.3*ItemTakeRadius);
			if ((GetCurrentDistanceToPickup() < ItemTakeRadius) && (!PickupItem->GetAttachParentActor()))
			{
				TakePickup();
			}
		}
	}
	else // If the pick up is close to player
	{
		PlayerCharacter->GetController()->SetIgnoreMoveInput(true);
		// Look at player when bot is running to him
		SetFocus(Cast<AActor>(PlayerCharacter), EAIFocusPriority::Gameplay);
		SurroundPlayer();

		if (GetCurrentDistanceToPlayer() < MaxDistFromPlayerToPickUp - 50.f) // If bot close enough to player we should try to drop pick up
		{
			// Drop the ball if this particular bot has it 
			if ((PickupItem->GetAttachParentActor()) && (Cast<APawn>(PickupItem->GetAttachParentActor()) == GetPawn()))
			{
				SetFocus(Cast<AActor>(PlayerCharacter), EAIFocusPriority::Gameplay);
				DropPickup();
			}
		}
	}
	//============================================// Main bot logic //============================================//
}

// Finding pickUp item by class APickUp and returning it
APickUp* AEnemyAIController::SetPickup()
{
	for (TActorIterator<APickUp>ActorIt(AActor::GetWorld()); ActorIt; ++ActorIt)
	{
		if (ActorIt)
		{
			return *ActorIt;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't find PickUp Item in EnemyController using iterator."));
			return NULL;
		}
	}
	return NULL;
}

// Talking for itself
float AEnemyAIController::GetCurrentDistanceToPickup()
{
	if (PickupItem)
		return GetPawn()->GetDistanceTo(Cast<AActor>(PickupItem));
	return -1.f;
}

// Talking for itself
float AEnemyAIController::GetDistanceFromPlayerToPickup()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn(); // Tried to use Cast from PlayerCharacter to pawn
																		   // but it didnt work. Use this solution for a time
	if (PlayerPawn && PickupItem)
	{
		return PlayerPawn->GetHorizontalDistanceTo(PickupItem);
	}
	else
	{
		return -1.f;
	}
}

// Talking for itself
float AEnemyAIController::GetCurrentDistanceToPlayer()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (GetPawn() && PlayerPawn)
		return GetPawn()->GetHorizontalDistanceTo(PlayerPawn);
	return -1.f;
}

// Attach pickUp item to bot
void AEnemyAIController::TakePickup()
{
	// Turn off all physics and velocity
	UPrimitiveComponent* PickupPrimitive = Cast<UPrimitiveComponent>(PickupItem->GetRootComponent());
	PickupPrimitive->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
	PickupPrimitive->SetSimulatePhysics(false);

	// Attach pick up to bot
	PickupItem->AttachToActor(GetPawn(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PickupItem->SetActorRelativeLocation(TakenItemPosition);
}

// Detach pick up item from current bot
void AEnemyAIController::DropPickup()
{
	// Detach pick up from bot
	PickupItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	// Turn physics on and push pick up a bot forward to player
	UPrimitiveComponent* PickupPrimitive = Cast<UPrimitiveComponent>(PickupItem->GetRootComponent());
	PickupPrimitive->SetSimulatePhysics(true);
	PickupPrimitive->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
	PickupPrimitive->AddImpulse(FVector(1000, 0, 0)); // Push pick up a bit forward. Should be used with smth like LookAtPlayer()
}

// Calculates value that bots should depart from adjacent bots
float AEnemyAIController::GetAngle()
{
	int32 Count = 0;
	for (TActorIterator<AEnemyAI>ActorIt(AActor::GetWorld()); ActorIt; ++ActorIt)
	{
		Count++;
	}
	if (Count == 0)
		return 0;
	return (FMath::DegreesToRadians(360.f / Count));
}

// Based on bot's unique number, returnes location that a bot should reach to surroun player
FVector AEnemyAIController::FindLocationAroundActor(AActor *SurroundedActor, float DistanceFromActor)
{
	float XCoord = 0;
	float YCoord = 0;
	float CircleAngle = GetAngle();

	XCoord = DistanceFromActor * FMath::Cos(UniqueBotIndex * CircleAngle);
	YCoord = DistanceFromActor * FMath::Sin(UniqueBotIndex * CircleAngle);
	
	FVector RelativeLocation = FVector(XCoord, YCoord, 0);

	return (SurroundedActor->GetActorLocation() + RelativeLocation);
}

// Checks if location is reachable
bool AEnemyAIController::isPositionReachable(FVector Position)
{
	UNavigationPath* NavPath = UNavigationSystem::FindPathToLocationSynchronously(
		GetWorld(), GetPawn()->GetActorLocation(),Position, NULL
	);
	
	if (!NavPath)
		return false;
	
	return (NavPath->IsValid());

	return false;
}

// If Surround Location is reachable, goes to it. If not, just move to player
void AEnemyAIController::SurroundPlayer()
{
	FVector CircleLocation = FindLocationAroundActor(PlayerCharacter, MaxDistToPlayer);

	if (isPositionReachable(CircleLocation))
	{
		MoveToLocation(CircleLocation, 0.1);
	}
	else
	{
		MoveToActor(PlayerCharacter, MaxDistToPlayer);
	}
}
