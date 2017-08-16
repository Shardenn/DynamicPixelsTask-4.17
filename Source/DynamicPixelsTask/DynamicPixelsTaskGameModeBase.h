// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DynamicPixelsTaskGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICPIXELSTASK_API ADynamicPixelsTaskGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ADynamicPixelsTaskGameModeBase(const FObjectInitializer& ObjectInitializer);
		
	virtual void StartPlay() override;
		
};
