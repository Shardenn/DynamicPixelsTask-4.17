// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/EnemyCharacter.h"
#include "../Public/PawnAIMovementComponent.h"
#include "../Public/AIMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EnemyMovementComponent = ObjectInitializer.CreateDefaultSubobject<UPawnAIMovementComponent>(this, FName("Movement component"));
	EnemyMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetMovementComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement comp is %s"), *GetMovementComponent()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: There is no Movement Component for this actor"));
	}
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyMovementComponent)
	{
		EnemyMovementComponent->AddInputVector(GetActorForwardVector(), true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tick: There is no Movement Component for this actor"));
	}
}

UPawnMovementComponent * AEnemyCharacter::GetMovementComponent() const
{
	return EnemyMovementComponent;
}
