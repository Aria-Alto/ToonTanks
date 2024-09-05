// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"


void AToonTankGameMode::ActorDied(AActor* deadActor) {
    if (deadActor == tank) {
        tank->HandleDestruction();
        if (tank->GetTankPlayerController()) {
            tank->DisableInput(tank->GetTankPlayerController());
            tank->GetTankPlayerController()->bShowMouseCursor = false;
        }
    } else if (ATower* destroyedTower = Cast<ATower>(deadActor)) {
        destroyedTower->HandleDestruction();
    }
}

void AToonTankGameMode::BeginPlay() {
    Super::BeginPlay();

    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}