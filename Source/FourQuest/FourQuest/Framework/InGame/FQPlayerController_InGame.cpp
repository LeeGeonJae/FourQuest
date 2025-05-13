// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"
#include "Blueprint/UserWidget.h"
#include "FQGameMode_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQSoul\Soul\FQSoulBase.h"
#include "FQUI/Player/FQPlayerHUDWidget.h"
#include "FQPlayerState_InGame.h"

AFQPlayerController_InGame::AFQPlayerController_InGame()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
	mPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget.Class);
}

void AFQPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();

	AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (FQPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild FQPlayerState_InGame"), __LINE__);
		return;
	}

	FQPlayerState->mSoulChangeDelegate.AddLambda([&](ESoulType NewSoulType)
		{
			UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
			FQPlayerHUDWidget->UpdateSoulIcon(NewSoulType);
		});

	UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
	if (FQPlayerHUDWidget)
	{
		FQPlayerHUDWidget->SetOwningActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild UFQPlayerHUDWidget"), __LINE__);
	}

	AFQGameMode_InGame* FQGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
	if (FQGameMode)
	{
		FQGameMode->GetPlayerHUDManager()->AddPlayerController(this, mPlayerHUDWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild AFQGameMode_InGame"), __LINE__);
	}
}

void AFQPlayerController_InGame::SetSoulType(ESoulType InSoulType)
{
	AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (FQPlayerState)
	{
		FQPlayerState->SetSoulType(InSoulType);
	}
}

ESoulType AFQPlayerController_InGame::GetSoulType() const
{
	AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (FQPlayerState)
	{
		ESoulType SoulType = FQPlayerState->GetSoulType();
		return SoulType;
	}
	return ESoulType::End;
}