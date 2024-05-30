// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen)
        {
            LoseScreen->AddToViewport();
        }
    }

    if (HUD) HUD->RemoveFromParent();
    GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &APlayerController::RestartLevel, RestartDelay, false);

}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (HUDClass)
    {
        HUD = CreateWidget(this, HUDClass);
        HUD->AddToViewport();
    }

}
