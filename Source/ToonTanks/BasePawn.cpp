// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	RootComponent = capsuleComp;
	
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	baseMesh->SetupAttachment(capsuleComp);
	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	projectileSpawnPoint->SetupAttachment(turretMesh);

}

void ABasePawn::HandleDestruction() {
	// TODO: Visual / sound affects
	if (explotionParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, explotionParticle, GetActorLocation(), GetActorRotation());
	}
}


void ABasePawn::RotateTurret(FVector lookAtTarget) {
	// to-target vector -> from turret location to mouse-cursor
	FVector toTarget = lookAtTarget - turretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0.f, toTarget.Rotation().Yaw, 0.f);

	turretMesh->SetWorldRotation(
		FMath::RInterpTo(
			turretMesh->GetComponentRotation(), 
			lookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			5.f)
	);
	
}

void ABasePawn::Fire() {
	// drawing debug sphere
    // DrawDebugSphere(GetWorld(), 
    //     projectileSpawnPoint->GetComponentLocation(), 
    //     25, 12, FColor::Red, false, 3.f);
	auto projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, projectileSpawnPoint->GetComponentLocation(), projectileSpawnPoint->GetComponentRotation());
	projectile->SetOwner(this);
}