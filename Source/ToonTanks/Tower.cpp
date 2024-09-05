// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATower::BeginPlay() {
    Super::BeginPlay();
    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::checkFireCondition, fireRate, true);
}

// Called every frame
void ATower::Tick(float DeltaTime) {
    if (InFireRange()) {
        // if in range, rotate turret torwards tank
        RotateTurret(tank->GetActorLocation());
    }
}

void ATower::HandleDestruction() {
    Super::HandleDestruction();
    Destroy();
}

void ATower::checkFireCondition() {
    if (InFireRange())
        Fire();
}

bool ATower::InFireRange() {
    if (tank) {
        float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
        return (distance <= fireRange);
    }
    return false;
}