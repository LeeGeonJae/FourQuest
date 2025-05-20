// Fill out your copyright notice in the Description page of Project Settings.

#include "FQGameMode_InGame.h"

#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

#include "FQ_GameInstance_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager/FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul/public/FQSoulBase.h"

AFQGameMode_InGame::AFQGameMode_InGame()
{
    mCurrentSoulType = ESoulType::Magic;
}

void AFQGameMode_InGame::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어를 추가했을 시에 카메라 분할(x) 세팅  
    if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
    {
        ViewportClient->SetForceDisableSplitscreen(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Faild Create Player : Current Player Count >= Max Player Count!!"), __LINE__);
    }

    mMainCamera = GetWorld()->SpawnActor<AFQMainCenterCamera>(AFQMainCenterCamera::StaticClass());
    if (mMainCamera == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] Faild Create MainCamera"), __LINE__);
    }

    // 화면 HUD Widget 세팅
	mPlayerHUDManager = GetWorld()->SpawnActor<AFQPlayerHUDManager>(AFQPlayerHUDManager::StaticClass());

    // 플레이어 컨트롤러 세팅
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        // F 키를 눌러서 다른 캐릭터 컨트롤러 생성 ( 임시 테스트용 )
        if (!PC->InputComponent)
        {
            PC->InputComponent = NewObject<UInputComponent>(PC, TEXT("PCInputComponent"));
            PC->InputComponent->RegisterComponent();
            PC->PushInputComponent(PC->InputComponent);
        }

        PC->InputComponent->BindKey(EKeys::F, IE_Pressed, this, &AFQGameMode_InGame::OnAnyKeyPressed);

        // 메인 카메라 세팅 ( 임시 테스트용 )
        if (mMainCamera)
        {
            PC->SetViewTargetWithBlend(mMainCamera);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] MainCamera Is nullptr"), __LINE__);
        }
    }
}

void AFQGameMode_InGame::OnAnyKeyPressed(FKey Key)
{
    TryCreatePlayerControllerFromKey(Key);
}

// 로컬 플레이어 생성 ( 임시 테스트용 함수 )
void AFQGameMode_InGame::TryCreatePlayerControllerFromKey(const FKey& PressedKey)
{
    if (mCreatedPlayerCount >= mMaxLocalPlayers)
    {
        UE_LOG(LogTemp, Warning, TEXT("[FQGameMode_InGame %d] Faild Create Player : Current Player Count >= Max Player Count!!"), __LINE__);
        return;
    }

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        const int32 ControllerId = mCreatedPlayerCount + 1;

        if (mSoulPlayersType.Num() > (uint8)mCurrentSoulType)
        {
            DefaultPawnClass = mSoulPlayersType[(uint8)mCurrentSoulType];
        }

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            mCreatedPlayerCount++;
            UE_LOG(LogTemp, Log, TEXT("Added Player %d"), ControllerId);

            // 카메라 세팅 ( 임시 테스트용 )
            if (mMainCamera)
            {
                NewPlayer->GetPlayerController(GetWorld())->SetViewTargetWithBlend(mMainCamera);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Not Find CameraActor"), __LINE__);
            }

            // 플레이어 스테이트 클래스에 소울 타입 지정
            AFQPlayerState_InGame* FQPlayerState = NewPlayer->GetPlayerController(GetWorld())->GetPlayerState<AFQPlayerState_InGame>();
            if (FQPlayerState)
            {
                FQPlayerState->SetSoulType(mCurrentSoulType);
                FQPlayerState->SetArmourType(EArmourType::End);
                FQPlayerState->SetSoulGauge(0);

                if (mCurrentSoulType == ESoulType::Knight)
                {
                    UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Create Player Soul Type Is Knight"), __LINE__);
                }
                else if (mCurrentSoulType == ESoulType::Magic)
                {
                    UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Create Player Soul Type Is Magic"), __LINE__);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Failed Create player for ControllerId %d"), __LINE__, ControllerId);
        }
    }
}