// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAIController.h"
#include "AIGroupManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachedPickup, AActor*, PickupItem);

class APickUp;

UCLASS()
class DYNAMICPIXELSTASK_API AAIGroupManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIGroupManager();

	UPROPERTY(EditAnywhere)
		float MinDistanceToPlayer = 250.f;
	UPROPERTY(EditAnywhere)
		float TakeItemDistance = 150.f;
	UPROPERTY(EditAnywhere)
		FVector TakenItemPosition = FVector(120.f, 0.f, 50.f);

	UPROPERTY(BlueprintAssignable)
		FReachedPickup BotReachedPickupItem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<AEnemyAIController*> BotControllers;
	UFUNCTION()
		void AttachItemToActor(AActor* Parent);

	float DistanceFromPlayerToPickup = 0.f;

	AActor* PlayerActor = NULL;
	APickUp* PickupItem = NULL;

	EPathFollowingStatus::Type BotMoveStatus;

	void InitializeBotsArray();
	
	void SetAllBotsRunToActor(AActor*, float AcceptanceDistance);
	APickUp* FindPickupItem();
};

