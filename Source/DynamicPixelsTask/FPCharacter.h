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

		virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	AFPCharacter();
	AFPCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*If we are carrying the sphere, it equals TRUE, otherwise == false*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ItemCarryDistance = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ItemCarryHeight = 50;

	bool IsItemEquipped = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlayerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* PlayerRoot;
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category=Camera)
		UCameraComponent* PlayerCamera;
	

	UFUNCTION()
		void MoveForward(float Amount);
	UFUNCTION()
		void MoveRight(float Amount);
	UFUNCTION()
		void OnJumpStart();
	UFUNCTION()
		void OnJumpEnd();
	/*Two next functions are for taking and throwing our sphere*/
	UFUNCTION()
		void TakeItem(FHitResult HitInfo);
	UFUNCTION()
		void ThrowItem();
	/*Function draws a line and checks if we are looking at the sphere and standing next to it*/
	UFUNCTION()
		void LineTrace();
	/*Function that checks if the sphere already equipped on the player*/
	UFUNCTION()
		void CheckEquipped();

	FORCEINLINE class USceneComponent* GetPlayerCamera() const { return PlayerCamera;  }
	
};