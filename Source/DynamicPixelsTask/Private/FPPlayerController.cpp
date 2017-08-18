// Fill out your copyright notice in the Description page of Project Settings.
#define PICKUP_CHANNEL       ECC_GameTraceChannel1

#include "../Public/FPPlayerController.h"
#include "FPCharacter.h"

void AFPPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedPawn = Cast<AFPCharacter>(InPawn);
		if (!ensure(PossesedPawn)) { return; }
	}
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted point in pickUp channel is %s"), *HitLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didnt hit the ball"));
	}
}

// Get screen location of crosshair
bool AFPPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		if (GetLookVectorHitLocation(LookDirection, HitLocation))
		{
			return true;
		}
	}

	return false;
}

// Get look direction according to crosshair position on the screen
bool AFPPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const
{
	FVector CameraLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraLocation, LookDirection);
}

// Trace a line into the world according to look direction
bool AFPPlayerController::GetLookVectorHitLocation(FVector CrosshairLookDirection, FVector &HittedPoint) const
{
	FHitResult ViewResult;
	auto StartPoint = PlayerCameraManager->GetCameraLocation(); 
	
	auto EndPoint = StartPoint + (TakeItemDistance * CrosshairLookDirection);
		
	if (GetWorld()->LineTraceSingleByChannel(ViewResult, StartPoint, EndPoint, PICKUP_CHANNEL))
	{
		HittedPoint = ViewResult.Location;
		return true;
	}
	return false;
}


