// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * Forward declarations
 */

class AAIGroupManager;

UCLASS()
class DYNAMICPIXELSTASK_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	
	// RULE : MaxDistToPlayer + MaxDistToPickUp < MaxPlayerAndItemDistance
	//Max distance to get to player
	UPROPERTY(EditAnywhere)
		float MaxDistToPlayer = 250.f;		
	// Acceptable radius to take the item
	UPROPERTY(EditAnywhere)
		float ItemTakeRadius = 150.0f;
	// Where should be item attached
	UPROPERTY(EditAnywhere)
		FVector TakenItemPosition = FVector(100,0,0);
	UPROPERTY(EditAnywhere)
		AAIGroupManager* Manager = NULL;

private:
	// Maximum distance from player to pick up to be counted as "close"
	float  MaxDistFromPlayerToPickUp = 400.f;
	// Each bot has its unoque number. Needed for location them around actors
	int32 UniqueBotIndex = 0;
	
	
	// Used to set up our pickup item into variable
	
	/*virtual APickUp* SetPickup();
	
	// Reference to out AFPCharacter class (Player's class)
	AFPCharacter* FPCharacter;

	// Always have a pointer to player character
	ACharacter* PlayerCharacter = NULL;

	// Pointer for pickup item in the scene
	APickUp* PickupItem = NULL;

	// These functions are talking for themselves
	/* They return -1 if something fails (nullptr for example)
	so we can add some check conditions before performing bot's logic 
	float GetCurrentDistanceToPickup();
	float GetDistanceFromPlayerToPickup();
	float GetCurrentDistanceToPlayer();

	// Attach pick up item to bot
	void TakePickup();
	// Detach pick up item from current bot
	void DropPickup();

	// Angle between two bots standing in the circle
	float GetAngle();
	// Location around actor in distance = DistanceFromActor based on bot's unique number
	FVector FindLocationAroundActor(AActor*, float DistanceFromActor);
	// Checks if position is reachable by the bot
	bool isPositionReachable(FVector Position);
	// Either go to position in circle or just move to actor
	void SurroundPlayer();
	*/
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
