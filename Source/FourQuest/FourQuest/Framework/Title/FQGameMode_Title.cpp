// Fill out your copyright notice in the Description page of Project Settings.


#include "FQGameMode_Title.h"
#include "Blueprint/UserWidget.h"
#include "FQUI\UI\FQTitleWidget.h"

AFQGameMode_Title::AFQGameMode_Title()
{
}

void AFQGameMode_Title::BeginPlay()
{
	Super::BeginPlay();

	if (mTitleWidgetClass)
	{
		mTitleWidgetHandle = CreateWidget<UFQTitleWidget>(GetWorld(), mTitleWidgetClass);
		mTitleWidgetHandle->SetOwningActor(this);
		mTitleWidgetHandle->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_Title %d] mTitleWidgetClass is nullptr"), __LINE__);
	}
}

void AFQGameMode_Title::MoveButton(const FInputActionValue& Value)
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

void AFQGameMode_Title::CancelInteraction()
{
	mTitleWidgetHandle->WidgetInput(EWidgetInputType::Cancel);
}

void AFQGameMode_Title::SelectInteraction()
{
	mTitleWidgetHandle->WidgetInput(EWidgetInputType::Select);
}
