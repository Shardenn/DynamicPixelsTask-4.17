// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EndGameTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DYNAMICPIXELSTASK_API UEndGameTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEndGameTrigger();

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
