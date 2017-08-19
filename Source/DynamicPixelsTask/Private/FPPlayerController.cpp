// Fill out your copyright notice in the Description page of Project Settings.
#define PICKUP_CHANNEL       ECC_GameTraceChannel1

#include "../Public/FPPlayerController.h"
#include "FPCharacter.h"



AFPPlayerController::AFPPlayerController()
{
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted point in pickUp channel is %s"), *HitLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didnt hit the ball"));
	}
	*/
}

void AFPPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedPawn = Cast<AFPCharacter>(InPawn);
		if (!ensure(PossesedPawn)) { return; }
	}
}

