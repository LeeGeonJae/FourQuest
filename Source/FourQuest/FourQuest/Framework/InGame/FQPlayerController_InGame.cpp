// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"
#include "Blueprint/UserWidget.h"
#include "FQGameMode_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"

AFQPlayerController_InGame::AFQPlayerController_InGame()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
	mPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget.Class);
}

void AFQPlayerController_InGame::BeginPlay()
{
	AFQGameMode_InGame* FQGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
	if (FQGameMode)
	{
		FQGameMode->GetPlayerHUDManager()->AddPlayerController(this, mPlayerHUDWidget);
	}
}
