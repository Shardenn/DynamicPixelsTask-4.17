// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"


// Sets default values
APickUp::APickUp(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PickupMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("Mesh"), false);
	RootComponent = PickupMesh;
	
}
// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	Cast<UPrimitiveComponent>(GetRootComponent())->SetSimulatePhysics(true);
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}