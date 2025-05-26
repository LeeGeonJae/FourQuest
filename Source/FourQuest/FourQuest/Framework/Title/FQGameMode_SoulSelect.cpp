// Fill out your copyright notice in the Description page of Project Settings.


#include "FQGameMode_SoulSelect.h"

#include "Blueprint/UserWidget.h"

#include "FQUI\UI\FQSoulSelectScreenWidget.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerController_InGame.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerState_InGame.h"

AFQGameMode_SoulSelect::AFQGameMode_SoulSelect()
{
}

void AFQGameMode_SoulSelect::BeginPlay()
{
	Super::BeginPlay();

	if (mSoulSelectWidgetClass)
	{
		mSoulSelectWidgetHandle = CreateWidget<UFQSoulSelectScreenWidget>(GetWorld(), mSoulSelectWidgetClass);
		mSoulSelectWidgetHandle->SetOwningActor(this);
		mSoulSelectWidgetHandle->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] mTitleWidgetClass is nullptr"), __LINE__);
	}
}

void AFQGameMode_SoulSelect::CreatePlayer()
{
    UGameInstance* MyGameInstance = GetGameInstance();
    if (!MyGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] MyGameInstance is nullptr!!"), __LINE__);
        return;
    }

    for (int32 i = 0; i < 4; i++)
    {
        int32 ControllerId = i;

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = MyGameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            // 플레이어 스테이트 클래스에 소울 타입 지정
            AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(NewPlayer->GetPlayerController(GetWorld()));
            if (!PC)
            {
                UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] AFQPlayerController_InGame Is nullptr!!"), __LINE__);
                return;
            }

            AFQPlayerState_InGame* FQPlayerState = PC->GetPlayerState<AFQPlayerState_InGame>();
            if (FQPlayerState)
            {
                FQPlayerState->SetSoulType(ESoulType::End);
                FQPlayerState->SetArmourType(EArmourType::End);
            }
        }
    }
}

void AFQGameMode_SoulSelect::MoveButton(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] SetButtonPosition Function Call"), __LINE__);
}

void AFQGameMode_SoulSelect::CancelInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] CancelInteraction Function Call"), __LINE__);
}

void AFQGameMode_SoulSelect::SelectInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] SelectInteraction Function Call"), __LINE__);
}