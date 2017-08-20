// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/PawnAIMovementComponent.h"

void UPawnAIMovementComponent::RequestDirectMove(const FVector & Target, bool)
{
	auto MoveVelocity = Target.GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("MoveVelocity is %s"), *MoveVelocity.ToString());
}
