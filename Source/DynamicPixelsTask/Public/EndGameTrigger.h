// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "EndGameTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DYNAMICPIXELSTASK_API UEndGameTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEndGameTrigger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bGameEnded = false;

	AActor* PlayerActor = NULL;

	void DestroyAllBots();
};
