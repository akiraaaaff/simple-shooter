// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay()
{
    Super::BeginPlay(); 

    if (AIBehavior)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); 
    }
    

}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ShooterCharacter = GetPawn<AShooterCharacter>();

    if (ShooterCharacter) return ShooterCharacter->IsDead();

    return true;
}
