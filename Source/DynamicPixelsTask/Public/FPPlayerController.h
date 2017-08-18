// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	/*X location of crosshair proportionally to screen size*/
	UPROPERTY(EditAnywhere)
		float CrosshairXLocation = 0.5;
	/*Y location of crosshair proportionally to screen size*/
	UPROPERTY(EditAnywhere)
		float CrosshairYLocation = 0.5;
	UPROPERTY(EditAnywhere)
		float TakeItemDistance = 170.0;

	void SetPawn(APawn* InPawn);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	bool GetSightRayHitLocation(FVector&) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector CrosshairLookDirection, FVector &HittedPoint) const;
};
