// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h" // dont see SetPawn to override, lets try include
#include "EnemyAI.h"
#include "EnemyAIController.generated.h"

/**
 * Forward declarations
 */
class APickUp;

UCLASS()
class DYNAMICPIXELSTASK_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AEnemyAI* PossesedEnemy = NULL;
	
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

private:
	float  MaxDistFromPlayerToPickUp = 400.f;
	
	// Used to set up our pickup item into variable
	virtual APickUp* SetPickup();

	// Always have a pointer to player character
	ACharacter* PlayerCharacter = NULL;

	// Pointer for pickup item in the scene
	APickUp* PickupItem = NULL;
	
	// These variables are talking for themselves I assume
	float CurrentDistanceToPlayer = 0.f;
	float CurrentDistanceToPickup = 0.f;
	float DistanceFromPlayerToPickup = 0.f;

	// These functions are talking for themselves
	/* They return -1 if something fails (nullptr for example)
	so we can add some check conditions before performing bot's logic */
	float GetCurrentDistanceToPickup();
	float GetDistanceFromPlayerToPickup();
	float GetCurrentDistanceToPlayer();

	// Attach pick up item to bot
	void TakePickup();
	// Detach pick up item from current bot
	void DropPickup();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
