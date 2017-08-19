// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "AIMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API UAIMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

	virtual void RequestDirectMove(const FVector&, bool) override;
};
