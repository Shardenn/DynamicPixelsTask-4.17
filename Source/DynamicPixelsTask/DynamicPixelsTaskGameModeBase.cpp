// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicPixelsTaskGameModeBase.h"
#include "Engine/Engine.h"
#include "FPCharacter.h"

ADynamicPixelsTaskGameModeBase::ADynamicPixelsTaskGameModeBase(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultPawnClass = AFPCharacter::StaticClass();
}

void ADynamicPixelsTaskGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor is Working."));
	}*/ // check if it's our custom gameMode running, not some default


}
