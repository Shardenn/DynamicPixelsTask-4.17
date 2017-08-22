// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyAIController.h"
#include "EngineUtils.h" // For actor iterator functions
#include "PickUp.h"
#include "FPCharacter.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PickupItem = SetPickup();
	PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	PickupItem = SetPickup();

	MaxDistFromPlayerToPickUp = MaxDistToPlayer + ItemTakeRadius + 50.f;

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

	DistanceFromPlayerToPickup = GetDistanceFromPlayerToPickup();
	CurrentDistanceToPickup = GetCurrentDistanceToPickup();
	

	//============================================// Main bot logic //============================================// 
	if (GetDistanceFromPlayerToPickup() > MaxDistFromPlayerToPickUp) // If ball is far from player
	{
		// TODO Turn on player movement
		
		if ((PickupItem->GetAttachParentActor()) &&
			(PickupItem->GetAttachParentActor()->GetClass()->IsChildOf(AEnemyAI::StaticClass())))  // If a bot already carrying the pick up
		{
			//MoveToActor(PlayerCharacter, MaxDistFromPlayerToPickUp - 50.f);
			FVector MoveLocation = FindLocationAroundActor(PlayerCharacter, MaxDistToPlayer);
			MoveToLocation(MoveLocation, 0.1);
			// Look at player when bot is running to him
			SetFocus(Cast<AActor>(PlayerCharacter), EAIFocusPriority::Move);
		}
		else // A parent of pick up is NOT a bot
		{
			ClearFocus(EAIFocusPriority::Move);
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
		// Surround player
		// TODO Turn off movement ASK about when to turn off movement
		
		//MoveToActor(PlayerCharacter, MaxDistFromPlayerToPickUp - 50.f);
		FVector MoveLocation = FindLocationAroundActor(PlayerCharacter, MaxDistToPlayer);
		// Look at player when bot is running to him
		SetFocus(Cast<AActor>(PlayerCharacter), EAIFocusPriority::Move);
		
		MoveToLocation(MoveLocation, 0.1);
		if (GetCurrentDistanceToPlayer() < MaxDistFromPlayerToPickUp - 50.f) // If bot close enough to player we should try to drop pick up
		{
			// TODO Turn off player movement
			// Drop the ball if this particular bot has it 
			if ((PickupItem->GetAttachParentActor()) && (Cast<APawn>(PickupItem->GetAttachParentActor()) == GetPawn()))
			{
				SetFocus(Cast<AActor>(PlayerCharacter));
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
	PickupPrimitive->AddImpulse(FVector(100, 0, 0)); // Push pick up a bit forward. Should be used with smth like LookAtPlayer()
}

float AEnemyAIController::GetAngle()
{
	int32 Count = 0;
	for (TActorIterator<AEnemyAI>ActorIt(AActor::GetWorld()); ActorIt; ++ActorIt)
	{
		Count++;
	}
	return (FMath::DegreesToRadians(360.f / Count));
}

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

