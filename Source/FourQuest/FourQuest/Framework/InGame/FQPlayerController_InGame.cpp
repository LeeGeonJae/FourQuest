// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"
#include "Blueprint/UserWidget.h"
#include "FQGameMode_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQSoul\Soul\FQSoulBase.h"
#include "FQUI/Player/FQPlayerHUDWidget.h"

AFQPlayerController_InGame::AFQPlayerController_InGame()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
	mPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget.Class);
	mSoulType = ESoulType::Knight;
}

void AFQPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();

	AFQSoulBase* SoulCharacterInterface = Cast<AFQSoulBase>(GetPawn());
	if (SoulCharacterInterface)
	{
		mSoulType = SoulCharacterInterface->GetSoulType();
	}

	Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget)->SetOwningActor(this);
	AFQGameMode_InGame* FQGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
	if (FQGameMode)
	{
		FQGameMode->GetPlayerHUDManager()->AddPlayerController(this, mPlayerHUDWidget);
	}
}

void AFQPlayerController_InGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// 자신이 가진 캐릭터의 소울 타입을 가져와서 세팅
	IFQSoulCharacterInterface* SoulCharacterInterface = Cast<IFQSoulCharacterInterface>(aPawn);
	if (SoulCharacterInterface == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Not Find SoulCharacterInterface!!"));
		return;
	}
	mSoulType = SoulCharacterInterface->GetSoulType();
}

void AFQPlayerController_InGame::SetSoulType(ESoulType InSoulType)
{
	mSoulType = InSoulType;
}

ESoulType AFQPlayerController_InGame::GetSoulType()
{
	return mSoulType;
}
