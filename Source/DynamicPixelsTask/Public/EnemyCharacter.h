// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyCharacter.generated.h"


// Forward declarations
class UPawnAIMovementComponent;

UCLASS()
class DYNAMICPIXELSTASK_API AEnemyCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UPawnAIMovementComponent* EnemyMovementComponent = NULL;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

};
