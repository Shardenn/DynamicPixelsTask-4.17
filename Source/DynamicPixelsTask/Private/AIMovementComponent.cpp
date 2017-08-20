// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/AIMovementComponent.h"


void UAIMovementComponent::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Move Component's owner is %s"), *GetOwner()->GetName());
}

void UAIMovementComponent::RequestDirectMove(const FVector &MoveVelocity, bool bForceMaxSpeed)
{
	auto EnemyForwardIntention = MoveVelocity.GetSafeNormal();
	
	UE_LOG(LogTemp, Warning, TEXT("Move velocity is %s"), *EnemyForwardIntention.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("%s vectoring at %s"), *EnemyName, *StMoveVelocity);
	
}
