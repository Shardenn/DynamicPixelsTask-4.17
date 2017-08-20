// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PawnAIMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API UPawnAIMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

		virtual void RequestDirectMove(const FVector& Target, bool) override;
	
	
};
