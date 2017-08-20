// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h" // For our iterator
#include "PickUp.h" // For finding our pickUp object by class as an actor
#include "Runtime/Engine/Public/EngineUtils.h" // For TActorIterator to find pickUp. Too much for one simple method to include (
#include "Runtime/Engine/Classes/GameFramework/Controller.h" // dont see SetPawn to override, lets try include
#include "EnemyCharacter.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	float AcceptableRadius = 250.0f;

private:
	AEnemyCharacter* PossesedEnemy = NULL;
	
	virtual AActor* FindPickup();

	//virtual void SetFocus(AActor*);

	virtual void BeginPlay() override;
	virtual void SetPawn(APawn*) override;
	virtual void Tick(float deltaTime) override;
};
