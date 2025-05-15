// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\Common.h"
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
    
    // GetSet Funtion
    FORCEINLINE class AFQPlayerHUDManager* GetPlayerHUDManager() const { return mPlayerHUDManager; }
    FORCEINLINE int32 GetPlayerCount() const { return mCreatedPlayerCount; }

protected:
    // Parent Class Funtion
    virtual void BeginPlay() override;

    // Common Funtion
    void OnAnyKeyPressed(FKey Key);                                 // Gamepad 입력을 감지하고 PlayerController를 생성 ( 임시 )
    void TryCreatePlayerControllerFromKey(const FKey& PressedKey);  // 입력을 분석하여 Gamepad Source를 식별 ( 임시 )

private:
    // 맵에 생성할 영혼 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul, Meta = (AllowPrivateAccess = "true"))
    TArray<TSubclassOf<class AFQSoulBase>> mSoulPlayersType;

    // 플레이어 HUD WIdget Manager
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQHUDWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AFQPlayerHUDManager> mPlayerHUDManager;

    // 로컬 플레이어 생성 ( 임시 )
    int32 mCreatedPlayerCount = 0;      // 이미 생성된 Player 수
    int32 mMaxLocalPlayers = 4;         // 최대 로컬 플레이어 수 (원하는 수만큼 설정 가능)
    ESoulType mCurrentSoulType;         // 생성할 영혼 타입
};
