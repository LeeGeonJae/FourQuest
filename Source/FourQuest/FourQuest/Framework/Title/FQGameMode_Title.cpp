// Fill out your copyright notice in the Description page of Project Settings.


#include "FQGameMode_Title.h"

#include "Blueprint/UserWidget.h"
#include "FQUI\UI\FQTitleScreenWidget.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"

AFQGameMode_Title::AFQGameMode_Title()
{
}

void AFQGameMode_Title::BeginPlay()
{
	Super::BeginPlay();

	if (mTitleWidgetClass)
	{
		mTitleWidgetHandle = CreateWidget<UFQTitleScreenWidget>(GetWorld(), mTitleWidgetClass);
		mTitleWidgetHandle->SetOwningActor(this);
		mTitleWidgetHandle->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_Title %d] mTitleWidgetClass is nullptr"), __LINE__);
	}
}

void AFQGameMode_Title::StartGame()
{
	if (mTargetLevelName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_Title] TargetLevelName is not set!"));
		return;
	}

	UGameplayStatics::OpenLevel(this, mTargetLevelName);
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_Title] StartGame: Opening level '%s'"), *mTargetLevelName.ToString());
}

void AFQGameMode_Title::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void AFQGameMode_Title::MoveButton(const FInputActionValue& Value, int32 ControllerId)
{
	FVector2D Direction = Value.Get<FVector2D>();
	if (Direction.X >= 0.8f)
	{
		mTitleWidgetHandle->WidgetInput(EWidgetInputType::Up);
	}
	if (Direction.X <= -0.8f)
	{
		mTitleWidgetHandle->WidgetInput(EWidgetInputType::Down);
	}
	if (Direction.Y >= 0.8f)
	{
		mTitleWidgetHandle->WidgetInput(EWidgetInputType::Right);
	}
	if (Direction.Y <= -0.8f)
	{
		mTitleWidgetHandle->WidgetInput(EWidgetInputType::Left);
	}
}

void AFQGameMode_Title::CancelInteraction(int32 ControllerId)
{
	mTitleWidgetHandle->WidgetInput(EWidgetInputType::Cancel);
}

void AFQGameMode_Title::SelectInteraction(int32 ControllerId)
{
	mTitleWidgetHandle->WidgetInput(EWidgetInputType::Select);

	ETitleButtonType ButtonType = mTitleWidgetHandle->GetSelectIndex();
	switch (ButtonType)
	{
	case ETitleButtonType::GameStart:
		StartGame();
		break;
	case ETitleButtonType::GameExit:
		ExitGame();
		break;
	}
}
