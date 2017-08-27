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
		float MinDistanceToPlayer = 200.f;
	UPROPERTY(EditAnywhere)
		float TakeItemDistance = 150.f;
	UPROPERTY(EditAnywhere)
		float PickupAcceptanceRadius = 150.f;
	UPROPERTY(EditAnywhere)
		FVector TakenItemPosition = FVector(120.f, 0.f, 50.f);
	
	UFUNCTION()
		void CheckReachedActor(AActor* MovingBot);
	
	UPROPERTY(BlueprintAssignable)
		FReachedPickup WhichActorBotReached;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<AEnemyAIController*> BotControllers;

	float DistanceFromPlayerToPickup = 0.f;
	float AngleRad = 0.f;
	int32 BotsSurroundedPlayer = 0;

	ACharacter* PlayerCharacter = NULL;
	APickUp* PickupItem = NULL;
	AActor* ReachedActor = NULL;
	AActor* CurrentTarget = NULL;

	EPathFollowingStatus::Type BotMoveStatus;

	AActor* SetAllBotsRunToActor(AActor*, float AcceptanceDistance);
	AActor* SurroundPlayer();
	void StopAllBotsMovement();
	void AttachItemToActor(AActor* Parent);
	void DetachItemFromActor();
	void CheckSurrounding();

	FVector LocationAroundPlayer(int32);

	void InitializeBotsArray();
	APickUp* FindPickupItem();
};

