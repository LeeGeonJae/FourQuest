// Fill out your copyright notice in the Description page of Project Settings.

#include "FQPlayerHUDManager.h"
#include "Components/HorizontalBox.h"
#include "FourQuest\FourQuest\Framework\InGame\FQ_GameInstance_InGame.h"

AFQPlayerHUDManager::AFQPlayerHUDManager()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerHUDWidget.WBP_PlayerHUDWidget_C"));
	if (PlayerHUDWidget.Class) // UPROPERTY로 노출된 UUserWidget* 클래스를 지정
	{
		mHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget.Class);
		if (mHUDWidget)
		{
			mHUDWidget->AddToViewport(); // 화면에 위젯 추가
			UE_LOG(LogTemp, Log, TEXT("Create HUD Widget And AddViewport"));
		}
	}
}

void AFQPlayerHUDManager::AddPlayerController(APlayerController* NewPlayer)
{
	if (!mHUDWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Is Not Valid HUDWidget"));
		return;
	}

	UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(mHUDWidget->GetWidgetFromName(TEXT("PlayerHUD")));
	if (!mHUDWidget || !HorizontalBox) return;

	if (UWorld* World = NewPlayer->GetWorld())
	{
		// 플레이어 전용 HUD 생성
		ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
		UUserWidget* PlayerHUD = CreateWidget<UUserWidget>(World, PlayerHUDWidget.Class);
		if (PlayerHUD)
		{
			// HorizontalBox에 추가
			HorizontalBox->AddChildToHorizontalBox(PlayerHUD);
		}

		// 플레이어 번호에 따른 HUD 저장
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(NewPlayer->Player);
		int32 ControllerId = LocalPlayer->GetControllerId();

		mPlayerHUDs.Add(ControllerId, PlayerHUD);
	}
}

void AFQPlayerHUDManager::BeginPlay()
{
    Super::BeginPlay();
}
