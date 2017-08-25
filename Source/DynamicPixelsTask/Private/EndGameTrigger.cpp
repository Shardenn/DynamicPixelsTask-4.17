// Fill out your copyright notice in the Description page of Project Settings.

#include "EndGameTrigger.h"
#include "Engine/TriggerBase.h"
#include "EngineUtils.h" // For actor iterator functions
#include "Engine.h"
#include "../Public/EnemyAI.h"

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

}

void UEndGameTrigger::DestroyAllBots()
{
	
}

