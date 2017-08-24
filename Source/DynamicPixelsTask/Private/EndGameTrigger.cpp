// Fill out your copyright notice in the Description page of Project Settings.

#include "EndGameTrigger.h"
#include "Engine/TriggerBase.h"
#include "Engine/World.h"
#include "../Public/EnemyAIController.h" // for finding bots by class in iterator
#include "EngineUtils.h" // For actor iterator functions
#include "Engine.h"

// Sets default values for this component's properties
UEndGameTrigger::UEndGameTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEndGameTrigger::BeginPlay()
{
	Super::BeginPlay();

	
	
	
}


// Called every frame
void UEndGameTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ATriggerBase* EndGameTrigger = NULL;
	EndGameTrigger = Cast<ATriggerBase>(GetOwner());
	
	PlayerActor = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (EndGameTrigger && PlayerActor)
	{
		if (EndGameTrigger->IsOverlappingActor(PlayerActor) && !bGameEnded)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapping player"));
			DestroyAllBots();
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "You win!");
		}
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("No trigger or player found"));
	}
}

void UEndGameTrigger::DestroyAllBots()
{
	for (TActorIterator<AEnemyAI>BotItr(GetWorld()); BotItr; ++BotItr)
	{
		if (BotItr)
		{
			BotItr->Destroy();
		}
	}
	bGameEnded = true;
}

