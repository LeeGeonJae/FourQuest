// Fill out your copyright notice in the Description page of Project Settings.


#include "FQGameMode_SoulSelect.h"
#include "Blueprint/UserWidget.h"
#include "FQUI\UI\FQSoulSelectWidget.h"

AFQGameMode_SoulSelect::AFQGameMode_SoulSelect()
{
}

void AFQGameMode_SoulSelect::BeginPlay()
{
	Super::BeginPlay();

	if (mSoulSelectWidgetClass)
	{
		mSoulSelectWidgetHandle = CreateWidget<UFQSoulSelectWidget>(GetWorld(), mSoulSelectWidgetClass);
		mSoulSelectWidgetHandle->SetOwningActor(this);
		mSoulSelectWidgetHandle->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] mTitleWidgetClass is nullptr"), __LINE__);
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