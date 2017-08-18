// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUp.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h" // for SetSimulatePhysics()
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h" 
#include "Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "FPCharacter.generated.h"


UCLASS()
class DYNAMICPIXELSTASK_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	AFPCharacter(const FObjectInitializer& ObjectInitializer);

	bool IsItemEquipped = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlayerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* PlayerRoot;
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category=Camera)
		UCameraComponent* PlayerCamera;
	
	
	void TakeItem(FHitResult HitInfo);
	void ThrowItem();
	void LineTrace();
	void CheckEquipped();

	FORCEINLINE class USceneComponent* GetPlayerCamera() const { return PlayerCamera;  }

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	
	void OnJumpStart();
	void OnJumpEnd();

	void CheckViewObjectIsPickup() const;
	
};