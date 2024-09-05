// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = projectileMesh;

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComponent"));
	movementComponent->InitialSpeed = initialSpeed;
	movementComponent->MaxSpeed = maxSpeed;

	tailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeTrail"));
	tailComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	if (launchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, launchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) {
	auto myOwner = this->GetOwner();
	if (myOwner == nullptr) {
		Destroy();
		return;
	}
	
	// controller
	auto myOwnerInstigator = myOwner->GetInstigatorController();

	if(otherActor && (otherActor != this) && (otherActor != myOwner)) {
		UGameplayStatics::ApplyDamage(otherActor, damage, myOwnerInstigator, this, UDamageType::StaticClass());
		if (hitParticles) {
			UGameplayStatics::SpawnEmitterAtLocation(this, hitParticles, GetActorLocation(), GetActorRotation());
		}
		if (hitSound) {
			UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
		}
	}
	Destroy();
}