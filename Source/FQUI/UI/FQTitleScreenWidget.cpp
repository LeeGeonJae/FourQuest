// Fill out your copyright notice in the Description page of Project Settings.


#include "FQTitleScreenWidget.h"

#include "Components\Image.h"
#include "Components/CanvasPanelSlot.h"


UFQTitleScreenWidget::UFQTitleScreenWidget()
    : mElapsedTime(0.f)
    , mCurrentFrameIndex(0)
    , mCurrentSelectIndex(ETitleButtonType::GameStart)
    , mSoulAnimations{}
    , mSelectButton()
    , mCurrentSelect()
{
}

void UFQTitleScreenWidget::WidgetInput(EWidgetInputType InputType)
{
    switch (InputType)
    {
    case EWidgetInputType::Left:
        MoveIndex(EWidgetInputType::Left);
        break;
    case EWidgetInputType::Right:
        MoveIndex(EWidgetInputType::Right);
        break;
    case EWidgetInputType::Up:
        MoveIndex(EWidgetInputType::Up);
        break;
    case EWidgetInputType::Down:
        MoveIndex(EWidgetInputType::Down);
        break;
    case EWidgetInputType::Select:
        SelectButton();
        break;
    case EWidgetInputType::Cancel:
        CancelButton();
        break;
    }
}

void UFQTitleScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Image Is Null!!"), __LINE__);
        return;
    }
}

void UFQTitleScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    mElapsedTime += InDeltaTime;
    if (mElapsedTime < 0.1f)
    {
        return;
    }

    // 애니메이션 플레이
    mElapsedTime = 0.f;
    if (mSoulAnimations.Num() > 0)
    {
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mSoulAnimations.Num();
        if (mCurrentSelect)
        {
            mCurrentSelect->SetBrushFromTexture(mSoulAnimations[mCurrentFrameIndex]);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mCurrentSelect Is Nullptr!!"), __LINE__);
        }
    }
}

void UFQTitleScreenWidget::MoveIndex(EWidgetInputType InputType)
{
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Is nullptr!!"), __LINE__);
        return;
    }

    if (InputType == EWidgetInputType::Up && mCurrentSelectIndex != ETitleButtonType::GameStart)
    {
        mCurrentSelectIndex = (ETitleButtonType)((uint8)(mCurrentSelectIndex) - 1);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
    }
    else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex != ETitleButtonType::GameExit)
    {
        mCurrentSelectIndex = (ETitleButtonType)((uint8)mCurrentSelectIndex + 1);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 100));
        }
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 100));
        }
    }
}

void UFQTitleScreenWidget::SelectButton()
{

}

void UFQTitleScreenWidget::CancelButton()
{

}
