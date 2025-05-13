// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMainCenterCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


AFQMainCenterCamera::AFQMainCenterCamera()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFQMainCenterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CenterLocation = GetCenterLocationOfPlayers();
	FVector NewLocation = FVector(CenterLocation.X, CenterLocation.Y, GetActorLocation().Z); // Z는 유지
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
}

FVector AFQMainCenterCamera::GetCenterLocationOfPlayers() const
{
    UWorld* World = GetWorld();
    if (!World) return FVector::ZeroVector;

    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    if (PlayerControllers.Num() == 0) return FVector::ZeroVector;

    FVector Sum = FVector::ZeroVector;
    int32 Count = 0;

    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            Sum += PC->GetPawn()->GetActorLocation();
            Count++;
        }
    }

    return Count > 0 ? (Sum / Count) : FVector::ZeroVector;
}

