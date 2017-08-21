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

	bool bItemEquipped = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlayerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* PlayerRoot;
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category=Camera)
		UCameraComponent* PlayerCamera;
	
	/*Where is crosshair on the screen horizontally, from 0 to 1*/
	UPROPERTY(EditAnywhere)
		float CrosshairXProportionalPos = 0.5;
	/*Where is crosshair on the screen vertically, from 0 to 1*/
	UPROPERTY(EditAnywhere)
		float CrosshairYProportionalPos = 0.5;
	/*How far we can stay from object and take it*/
	UPROPERTY(EditAnywhere)
		float MaxTakeDistance = 160.0;
	/*How fast will item fly after we throw it*/
	UPROPERTY(EditAnywhere)
		float ThrowImpulse = 100000;
	/*Where taken item should be located relative to player*/
	UPROPERTY(EditAnywhere)
		FVector TakenItemPosition = FVector(160, 0, 0);



	void TakeItem(FHitResult HitInfo);
	void ThrowItem();
	void CheckEquipped();

	FORCEINLINE class USceneComponent* GetPlayerCamera() const { return PlayerCamera;  }

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	
	void OnJumpStart();
	void OnJumpEnd();

	/*Function needed to start checking viewd object if we pressed LMB*/
	void CheckViewedObject();
	/*Where crosshiar is looking*/
	bool GetCrosshairLookDirection(FVector& LookDirection) const;
	/*Contains information about hitted objects if they are in PICKUP_COLLISION channel*/
	bool GetViewedObject(FVector LookDirection, FHitResult&) const;
};