// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
// #include "Engine/EngineTypes.h"



ATank::ATank() {
    // super.
    springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);
    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	playerControllerRef = Cast<APlayerController>(GetController());
    if(playerControllerRef == nullptr) {
        UE_LOG(LogTemp, Display, TEXT("PlayerController has not been initialized"));
    }
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // checking the linetrace from the camera to the cursor in order to aim the turret
    FHitResult hitResult;
    playerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);
    // drawing debug sphere
    DrawDebugSphere(
        GetWorld(), 
        hitResult.ImpactPoint, 
        25, 12, FColor::Red, false, -1.f);
}

void ATank::Move(float value) {
    if (value == 0) { return;}
    // x = deltalocation * delta time * speed

    FVector deltaLocation {value * speed, 0.f, 0.f};
    deltaLocation.X *= UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(deltaLocation, true);
}

void ATank::Turn(float value) {

    FRotator deltaRotation {0.f};
    // Yaw = value * delta-time * turn-rate
    deltaRotation.Yaw += value * turnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation( deltaRotation,true);
}