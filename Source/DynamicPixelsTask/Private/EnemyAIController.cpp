// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyAIController.h"
#include "AIGroupManager.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	AAIController::OnMoveCompleted(RequestID, Result);
	if (Manager)
	{
		//Manager->WhichActorBotReached.Broadcast(GetPawn());
		Manager->CheckReachedActor(GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Manager"));
	}
	if(!Manager->WhichActorBotReached.IsBound())
		UE_LOG(LogTemp, Warning, TEXT("Delegate is not bound"));
}
