// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/CustomCharMovementComponent.h"

void UCustomCharMovementComponent::RequestDirectMove(const FVector &MoveVector, bool bMaxSpeed)
{
	auto MoveVelocity = MoveVector.GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("Inside CHARACTER moCompo. RequestDirectMove is %s"), *MoveVelocity.ToString());

}
