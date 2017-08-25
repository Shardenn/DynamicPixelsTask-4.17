// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGroupManager.h"
#include "EngineUtils.h"
#include "PickUp.h"

// Sets default values
AAIGroupManager::AAIGroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotReachedPickupItem.AddDynamic(this, &AAIGroupManager::AttachItemToActor); // TODO call this thing from a bot that first
																				// reaches pickup. Also undrestand how to fo it (
}

// Called when the game starts or when spawned
void AAIGroupManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeBotsArray();
	
	PlayerActor = AActor::GetWorld()->GetFirstPlayerController()->GetPawn();
	PickupItem = FindPickupItem();

	/*
	if (BotControllers[0]->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle"));
	}
	*/
}

// Called every frame
void AAIGroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BotMoveStatus = BotControllers[0]->GetMoveStatus();
	
	DistanceFromPlayerToPickup = PlayerActor->GetHorizontalDistanceTo(PickupItem);

	if (DistanceFromPlayerToPickup > MinDistanceToPlayer + TakeItemDistance + 10.f)
	{
		SetAllBotsRunToActor(PickupItem, 150.f);
	}
	
}

void AAIGroupManager::InitializeBotsArray()
{
	
	for (TActorIterator<AEnemyAIController>BotItr(AActor::GetWorld()); BotItr; ++BotItr)
	{
		if (BotItr->IsActorInitialized() && BotItr->HasActorBegunPlay())
		{
			BotControllers.Add(*BotItr);
		}
	}
	return;
}

void AAIGroupManager::SetAllBotsRunToActor(AActor * Target, float AcceptanceDistance)
{
	for (auto BotItr : BotControllers)
	{
		if (BotItr->GetMoveStatus() < EPathFollowingStatus::Moving)
		{
			BotItr->MoveToActor(Target, AcceptanceDistance);
			UE_LOG(LogTemp, Warning, TEXT("%s started run to ball"), *BotItr->GetName());
		}
	}
	return;
}

APickUp* AAIGroupManager::FindPickupItem()
{
	for (TActorIterator<APickUp>ActorItr(AActor::GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsActorInitialized() && ActorItr->HasActorBegunPlay())
			return *ActorItr;
	}
	return NULL;
}

void AAIGroupManager::AttachItemToActor(AActor* Bot)
{
	// Turn off all physics and velocity
	UPrimitiveComponent* PickupPrimitive = Cast<UPrimitiveComponent>(PickupItem->GetRootComponent());
	PickupPrimitive->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
	PickupPrimitive->SetSimulatePhysics(false);

	if (!PickupItem->GetAttachParentActor())
	{
		PickupItem->AttachToActor(Bot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PickupItem->SetActorRelativeLocation(TakenItemPosition);
	}
}