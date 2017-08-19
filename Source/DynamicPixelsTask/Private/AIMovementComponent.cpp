// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/AIMovementComponent.h"


void UAIMovementComponent::BeginPlay()
{
}

void UAIMovementComponent::RequestDirectMove(const FVector &MoveVelocity, bool bForceMaxSpeed)
{
	auto EnemyForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto EnemyForwardIntention = MoveVelocity.GetSafeNormal();
	
	
	
	//UE_LOG(LogTemp, Warning, TEXT("%s vectoring at %s"), *EnemyName, *StMoveVelocity);
}
