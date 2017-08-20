// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API UCustomCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual void RequestDirectMove(const FVector&, bool) override;
	
	
};
