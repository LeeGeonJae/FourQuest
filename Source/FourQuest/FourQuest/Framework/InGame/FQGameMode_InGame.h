// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameMode_InGame.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_InGame : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFQGameMode_InGame();

    virtual void BeginPlay() override;

    // Gamepad 입력을 감지하고 PlayerController를 생성
    UFUNCTION()
    void OnAnyKeyPressed(FKey Key);

    // 입력을 분석하여 Gamepad Source를 식별
    void TryCreatePlayerControllerFromKey(const FKey& PressedKey);

    FORCEINLINE class AFQPlayerHUDManager* GetPlayerHUDManager() const { return mPlayerHUDManager; }
    FORCEINLINE int32 GetPlayerCount() const { return CreatedPlayerCount; }

private:
	TObjectPtr<class AFQPlayerHUDManager> mPlayerHUDManager;

    // 이미 생성된 Player 수
    int32 CreatedPlayerCount = 0;

    // 최대 로컬 플레이어 수 (원하는 수만큼 설정 가능)
    int32 MaxLocalPlayers = 4;
};
