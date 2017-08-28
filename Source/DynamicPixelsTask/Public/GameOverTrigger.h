// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOverTrigger.generated.h"

class UBoxComponent;
class APickUp;

UCLASS()
class DYNAMICPIXELSTASK_API AGameOverTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOverTrigger(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	bool bGameEnded = false;

	APickUp* Ball = NULL;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
