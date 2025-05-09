// Fill out your copyright notice in the Description page of Project Settings.

#include "FQGameMode_InGame.h"
#include "FQ_GameInstance_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager/FQPlayerHUDManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/LocalPlayer.h"

AFQGameMode_InGame::AFQGameMode_InGame()
{
}

void AFQGameMode_InGame::BeginPlay()
{
    Super::BeginPlay();

	mPlayerHUDManager = GetWorld()->SpawnActor<AFQPlayerHUDManager>(AFQPlayerHUDManager::StaticClass());

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (!PC->InputComponent)
        {
            PC->InputComponent = NewObject<UInputComponent>(PC, TEXT("PCInputComponent"));
            PC->InputComponent->RegisterComponent();
            PC->PushInputComponent(PC->InputComponent);
        }

        PC->InputComponent->BindKey(EKeys::F, IE_Pressed, this, &AFQGameMode_InGame::OnAnyKeyPressed);
    }
}

void AFQGameMode_InGame::OnAnyKeyPressed(FKey Key)
{
    TryCreatePlayerControllerFromKey(Key);
}

void AFQGameMode_InGame::TryCreatePlayerControllerFromKey(const FKey& PressedKey)
{
    if (CreatedPlayerCount >= MaxLocalPlayers)
    {
        UE_LOG(LogTemp, Warning, TEXT("Max player count reached."));
        return;
    }

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        const int32 ControllerId = CreatedPlayerCount + 1;

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            CreatedPlayerCount++;
            UE_LOG(LogTemp, Log, TEXT("Added Player %d"), ControllerId);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create player for ControllerId %d"), ControllerId);
        }
    }
}
