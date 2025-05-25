// Fill out your copyright notice in the Description page of Project Settings.

#include "FQGameMode_InGame.h"

#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

#include "FQ_GameInstance_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FQPlayerController_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager/FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul/public/FQSoulBase.h"

AFQGameMode_InGame::AFQGameMode_InGame()
{
    DefaultPawnClass = nullptr;
}

void AFQGameMode_InGame::BeginPlay()
{
    Super::BeginPlay();

    // 카메라 생성 및 세팅
    SettingCamera();
    // 플레이어 생성
    CreatePlayer();
}

void AFQGameMode_InGame::SettingCamera()
{
    // 로컬 플레이어를 추가했을 시에 카메라 분할(x) 세팅  
    if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
    {
        ViewportClient->SetForceDisableSplitscreen(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Faild Create Player : Current Player Count >= Max Player Count!!"), __LINE__);
    }

    // 메인 카메라 생성
    mMainCamera = GetWorld()->SpawnActor<AFQMainCenterCamera>(AFQMainCenterCamera::StaticClass());
    if (mMainCamera == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] Faild Create MainCamera"), __LINE__);
    }

    // 화면 HUD Widget 세팅
    mPlayerHUDManager = GetWorld()->SpawnActor<AFQPlayerHUDManager>(AFQPlayerHUDManager::StaticClass());
}

void AFQGameMode_InGame::CreatePlayer()
{
    UFQ_GameInstance_InGame* GameInstance = Cast<UFQ_GameInstance_InGame>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] UFQ_GameInstance_InGame Is Not Vaild!!"), __LINE__);
        return;
    }

    auto PlayerInfomations = GameInstance->GetLocalMultiPlayerInfomation();

    for (auto& Infomation : PlayerInfomations)
    {
        const int32 ControllerId = Infomation.Key;
        FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation = Infomation.Value;

        if (mSoulPlayersType.Num() > (uint8)PlayerInfomation.mSoulType)
        {
            DefaultPawnClass = mSoulPlayersType[(uint8)PlayerInfomation.mSoulType]; 
        }

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Added Player %d"), __LINE__, ControllerId);

            // 플레이어 스테이트 클래스에 소울 타입 지정
            AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(NewPlayer->GetPlayerController(GetWorld()));
            PC->UpdateHUDSetting();

            AFQPlayerState_InGame* FQPlayerState = PC->GetPlayerState<AFQPlayerState_InGame>();
            if (FQPlayerState)
            {
                FQPlayerState->SetSoulType(PlayerInfomation.mSoulType);
                FQPlayerState->SetArmourType(EArmourType::End);
                FQPlayerState->SetSoulGauge(0);

                if (PlayerInfomation.mSoulType == ESoulType::Knight)
                {
                    UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Create Player Soul Type Is Knight"), __LINE__);
                }
                else if (PlayerInfomation.mSoulType == ESoulType::Magic)
                {
                    UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Create Player Soul Type Is Magic"), __LINE__);
                }
            }
        }
        else if (ControllerId == 0)         // 0번째 플레이어 컨트롤러
        {
            AFQPlayerController_InGame* PC = GetWorld()->GetFirstPlayerController<AFQPlayerController_InGame>();

            // 기존 Pawn 제거 (선택 사항)
            if (PC->GetPawn())
            {
                PC->GetPawn()->Destroy();
            }

            // 새로운 캐릭터 생성
            FVector SpawnLocation = FVector::ZeroVector; // 위치 조정 가능
            FRotator SpawnRotation = FRotator::ZeroRotator;
            AFQSoulBase* NewPawn = GetWorld()->SpawnActor<AFQSoulBase>(mSoulPlayersType[(uint8)PlayerInfomation.mSoulType], SpawnLocation, SpawnRotation);
            if (!NewPawn)
            {
                UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d]Failed to spawn Pawn for ControllerId %d"), __LINE__, ControllerId);
                return;
            }

            // Possess 처리
            PC->Possess(NewPawn);
            PC->UpdateHUDSetting();

            // PlayerState 설정
            if (AFQPlayerState_InGame* PS = PC->GetPlayerState<AFQPlayerState_InGame>())
            {
                PS->SetSoulType(PlayerInfomation.mSoulType);
                PS->SetArmourType(EArmourType::End);
                PS->SetSoulGauge(0);
            }
            UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Added Player %d"), __LINE__, ControllerId);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Failed Create player for ControllerId %d"), __LINE__, ControllerId);
        }
    }
}