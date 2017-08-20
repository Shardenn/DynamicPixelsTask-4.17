// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyAIController.h"
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::SetPawn(APawn * OurPawn)
{
	Super::SetPawn(OurPawn);


	if (OurPawn)
	{
		auto PossesedBot = Cast<AEnemyCharacter>(OurPawn);
		if (!ensure(PossesedBot)) { return; }

		UE_LOG(LogTemp, Warning, TEXT("Possesed bot that has ID %s"), *(PossesedBot->GetName()));
	}
	else
	{

	}
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto PlayerPawn = AActor::GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* PossesedBot = GetPawn();
	SetPawn(PossesedBot);

	if (!PlayerPawn || !PossesedBot)
	{	return;	}
	
	auto PlayerActor = Cast<AActor>(PlayerPawn);

	AEnemyAIController* OurController = this;
	MoveToActor(PlayerActor);

	
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

