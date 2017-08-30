// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAIController.h"
#include "AIGroupManager.generated.h"

class APickUp;

UCLASS()
class DYNAMICPIXELSTASK_API AAIGroupManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AAIGroupManager();

	UPROPERTY(EditAnywhere)
		float MinDistanceToPlayer = 200.f;
	UPROPERTY(EditAnywhere)
		float PickupAcceptanceRadius = 0.f;
	UPROPERTY(EditAnywhere)
		float TakeItemDistance = 130.f;
	UPROPERTY(EditAnywhere)
		FVector TakenItemPosition = FVector(120.f, 0.f, 50.f);
	
	UFUNCTION()
		void CheckReachedActor(AActor* MovingBot);
	void EndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<AEnemyAIController*> BotControllers;

	float DistanceFromPlayerToPickup = 0.f;
	int32 BotsSurroundedPlayer = 0;
	FVector LastPlayerPosition = FVector::ZeroVector;

	ACharacter* PlayerCharacter = NULL;
	APickUp* PickupItem = NULL;
	AActor* ReachedActor = NULL;
	AActor* CurrentTarget = NULL;

	AActor* SetAllBotsRunToActor(AActor*, float AcceptanceDistance);
	AActor* SurroundPlayer();
	void StopAllBotsMovement();
	void AttachItemToActor(AActor* Parent);
	void DetachItemFromActor();
	void CheckSurrounding();
	void SetAllBotsLookAtPlayer();
	void TurnOffPlayerMovement();
	bool IsPositionReachable(FVector Start, FVector Finish);

	FVector LocationAroundPlayer(int32);

	void InitializeBotsArray();
	APickUp* FindPickupItem();
};

