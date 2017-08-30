// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "PickUp.generated.h"

UCLASS()
class DYNAMICPIXELSTASK_API APickUp : public AActor
{
	GENERATED_BODY()
	
private:
	// Sets default values for this actor's properties
	APickUp(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void TurnPhysicsOn(bool TurnBool);
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;
};