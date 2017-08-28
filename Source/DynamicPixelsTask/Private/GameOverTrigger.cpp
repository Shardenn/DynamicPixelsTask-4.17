// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverTrigger.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../Public/EnemyAI.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine.h"
#include "PickUp.h"
#include "AIGroupManager.h"

// Sets default values
AGameOverTrigger::AGameOverTrigger(const FObjectInitializer &ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision area"));
	RootComponent = CollisionComponent;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGameOverTrigger::OnActorOverlap);
}


// Called when the game starts or when spawned
void AGameOverTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameOverTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameOverTrigger::OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	if ((OtherActor) && (OtherActor == AActor::GetWorld()->GetFirstPlayerController()->GetPawn()) 
		&& (OtherComponent != NULL) && (!bGameEnded))
	{
		for (TActorIterator<AAIGroupManager>ManagerItr(AActor::GetWorld()); ManagerItr; ++ManagerItr)
			ManagerItr->EndGame();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "You Win!");
		bGameEnded = true;
	}
}