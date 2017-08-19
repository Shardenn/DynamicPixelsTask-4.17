// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyAIController.h"
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("AI Controller is possesing smth"));
}

void AEnemyAIController::SetPawn(APawn * OurPawn)
{
	Super::SetPawn(OurPawn);

	if (OurPawn)
	{
		auto PossesedBot = Cast<AEnemyCharacter>(OurPawn);
		if (!ensure(PossesedBot)) { return; }

		//UE_LOG(LogTemp, Warning, TEXT("Possesed bot that has ID %s"), *(PossesedBot->GetName()))
		
	}
	else
	{

	}
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto PlayerActor = AActor::GetWorld()->GetFirstPlayerController()->GetPawn();
	auto PossesedBot = AController::GetPawn();

	if (!PlayerActor || !PossesedBot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error in AIController - no Player or Possesed bot"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Possesed pawn is %s"), *(PossesedBot->GetName()));

	MoveToActor(PlayerActor, 150);

	
	
	/*
	UE_LOG(LogTemp, Warning, TEXT("Possesed bot that has ID %s"), *(PossesedBot->GetName()));
	UE_LOG(LogTemp, Warning, TEXT("Possesed bot that has ID %s"), *(PlayerActor->GetName()));
	
	UE_LOG(LogTemp, Warning, TEXT("Bot's location %s"), *PossesedBot->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Player's location %s"), *PlayerActor->GetActorLocation().ToString());
	*/
	
}

AActor* AEnemyAIController::FindPickup()
{
	/*for (TActorIterator<AStaticMeshActor*> ActorIt(AActor::GetWorld()); ActorIt; ++ActorIt)
	{

	}
	*/
	return NULL;
}

//void AEnemyAIController::SetFocus(AActor *)


void AEnemyAIController::ReachActor(AActor* Target, APawn* Bot)
{
	AAIController::SetFocus(Target);
	MoveToActor(Target);
	UE_LOG(LogTemp, Warning, TEXT("Possesed bot that has ID %s"), *(Target->GetName()));
	//UE_LOG(LogTemp, Warning, TEXT("Inside reachActor method %s"), *(Bot->GetActorForwardVector().ToString()));
}

