// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"


void AToonTankGameMode::ActorDied(AActor* deadActor) {
    if (deadActor == tank) {
        tank->HandleDestruction();
        if (toonTanksPlayerController) {
            toonTanksPlayerController->SetPlayerEnabledState(false);
            GameOver(false);
        }
    } else if (ATower* destroyedTower = Cast<ATower>(deadActor)) {
        destroyedTower->HandleDestruction();
        targetTowers --;
        if ( targetTowers == 0 ) { GameOver(true);}
    }
}

void AToonTankGameMode::BeginPlay() {
    Super::BeginPlay();
    
    HandleGameStart();
}

void AToonTankGameMode::HandleGameStart() {
    targetTowers =GetTargetTowerCount();
    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    toonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (toonTanksPlayerController) {
        toonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle playerEnableTimerHandle;
        FTimerDelegate playerEnableTimerDelegate = FTimerDelegate::CreateUObject(toonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(playerEnableTimerHandle, playerEnableTimerDelegate, startDelay, false);
    }
}

int AToonTankGameMode::GetTargetTowerCount() {
    TArray<AActor*> towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);
    return towers.Num();
}