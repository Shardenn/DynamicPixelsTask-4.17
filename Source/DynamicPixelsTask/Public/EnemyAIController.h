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
	
	UPROPERTY(EditAnywhere)
		AAIGroupManager* Manager = NULL;

private:
	
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
