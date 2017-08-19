// Fill out your copyright notice in the Description page of Project Settings.
#define COLLISION_PICKUP       ECC_GameTraceChannel1

#include "FPCharacter.h"
#include "../Source/DynamicPixelsTask/Public/FPPlayerController.h"

// Sets default values
AFPCharacter::AFPCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/*Player root component*/
	PlayerRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, FName("RootComponent"), false);
	PlayerRoot->SetupAttachment((USceneComponent*)GetCapsuleComponent()); // didnt take UCapsuleComponent for USceneComp
	PlayerRoot->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	
	/*Player's eyes*/
	PlayerCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("PlayerCamera"), false);
	PlayerCamera->SetupAttachment(PlayerRoot);
	PlayerCamera->bUsePawnControlRotation = true;

	float CameraHeight = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	PlayerCamera->SetRelativeLocationAndRotation(FVector(40.0f, 0.0f, CameraHeight), FRotator(0.0f, 0.0f, 0.0f)); // set on the top
																								//of actor and a bit forward
}


// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//print("We are using FPCharacter"); // check if is our custom character is created, not some default
}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	/*Binding movement axes*/
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);
	/*Binding mouse axes to look around*/
	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacter::AddControllerPitchInput);
	/*Binding SpaceBar to jump*/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::OnJumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPCharacter::OnJumpEnd);
	/*Binding LMB to pickup our cube*/
	PlayerInputComponent->BindAction("TakeItem", IE_Pressed, this, &AFPCharacter::CheckViewedObject);
	PlayerInputComponent->BindAction("ThrowItem", IE_Pressed, this, &AFPCharacter::CheckEquipped);
}
/*Function for forward-backward movement*/
void AFPCharacter::MoveForward(float Amount)
{
	if (Controller && Amount)
	{
		AddMovementInput(GetActorForwardVector(), Amount, false);
	}
	
}
/*Function for left-right movement*/
void AFPCharacter::MoveRight(float Amount)
{
	if (Controller && Amount)
	{
		AddMovementInput(GetActorRightVector(), Amount, false);
	}
}
/*Function to set our player jumping*/
void AFPCharacter::OnJumpStart()
{
	bPressedJump = true;
}
/*Function to set our player not jumoing*/
void AFPCharacter::OnJumpEnd()
{
	bPressedJump = false;
}

void AFPCharacter::CheckViewedObject()
{
	if (!IsItemEquipped)
	{
		FVector LookDirection;
		if (GetCrosshairLookDirection(LookDirection))
		{
			FHitResult ViewedResult;
			if (GetViewedObject(LookDirection, ViewedResult))
				TakeItem(ViewedResult);
		}
	}
}

bool AFPCharacter::GetCrosshairLookDirection(FVector & LookDirection) const
{
	int32 ViewportXSize, ViewportYSize;
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->GetViewportSize(ViewportXSize, ViewportYSize);

	auto CrosshairScreenPosition = FVector2D(ViewportXSize * CrosshairXProportionalPos, ViewportYSize * CrosshairYProportionalPos);

	FVector CameraLocation; // To be discarded
	if (PlayerController->DeprojectScreenPositionToWorld(
		CrosshairScreenPosition.X,
		CrosshairScreenPosition.Y,
		CameraLocation,
		LookDirection)
		)
	{
		UE_LOG(LogTemp, Warning, TEXT("Crosshair look direction is %s"), *LookDirection.ToString());
		return true;
	}
	return false;
}

bool AFPCharacter::GetViewedObject(FVector LookDirection, FHitResult & ViewResult) const
{
	APlayerController * PlayerController = Cast<APlayerController>(GetController());
	auto StartPoint = PlayerController->PlayerCameraManager->GetCameraLocation();
	auto EndPoint = StartPoint + (MaxTakeDistance * LookDirection);

	FHitResult BufferHitResult;
	if (GetWorld()->LineTraceSingleByChannel(BufferHitResult, StartPoint, EndPoint, COLLISION_PICKUP))
	{
		ViewResult = BufferHitResult;
		UE_LOG(LogTemp, Warning, TEXT("Crosshair look direction is %s"), *(BufferHitResult.GetActor()->GetName()));
		return true;
	}
	return false;
}



/*Function that allows us to take our pickUp item*/
void AFPCharacter::TakeItem(FHitResult HitInfo)
{
	AActor *ItemTemp = HitInfo.GetActor(); // temporary variable for Sphere actor
	UE_LOG(LogTemp, Warning, TEXT("Normalized view vector is %s"), *HitInfo.Normal.ToString());
	IsItemEquipped = true;
	
	Cast<UPrimitiveComponent>(ItemTemp->GetRootComponent())->SetSimulatePhysics(false); // Turning physics off
	/*Attaching pick Up to player. X and Z location of pickUp are editable from character blueprint*/
	ItemTemp->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ItemTemp->SetActorRelativeLocation(TakenItemPosition);
}

void AFPCharacter::CheckEquipped()
{
	if (IsItemEquipped)
		AFPCharacter::ThrowItem();
	return;
}

void AFPCharacter::ThrowItem() // REMINDER: to get attached Sphere use this->GetAttachedActors()
{
	IsItemEquipped = false;

	/*Getting all attached actors and takink [0] element as our picked up object*/
	TArray<AActor*>PlayerAttachedActors;
	this->GetAttachedActors(PlayerAttachedActors);
	AActor* ItemTemp = PlayerAttachedActors[0];  

	/*Enabling physics and detaching item*/
	ItemTemp->SetActorEnableCollision(true);
	UPrimitiveComponent* TempPrimitive = Cast<UPrimitiveComponent>(ItemTemp->GetRootComponent());
	TempPrimitive->SetSimulatePhysics(true);
	ItemTemp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	/*Throwing object in view direction*/
	float ThrowImpulse = 100000.0;
	FVector ThrowVector;
	if(GetCrosshairLookDirection(ThrowVector))
		TempPrimitive->AddImpulse(ThrowVector * ThrowImpulse);
}