// Fill out your copyright notice in the Description page of Project Settings.


#include "FQTitleWidget.h"

#include "Components\Image.h"
#include "Components/CanvasPanelSlot.h"


UFQTitleWidget::UFQTitleWidget()
    : mElapsedTime(0.f)
    , mCurrentFrameIndex(0)
    , mCurrentSelectIndex(0)
    , mSoulAnimations{}
    , mSelectButton()
    , mCurrentSelect()
    , mMaxSelectIndex(2)
{
}

void UFQTitleWidget::WidgetInput(EWidgetInputType InputType)
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

void UFQTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Image Is Null!!"), __LINE__);
        return;
    }
}

void UFQTitleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UFQTitleWidget::MoveIndex(EWidgetInputType InputType)
{
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        return;
    }

    if (InputType == EWidgetInputType::Up && mCurrentSelectIndex > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("[UFQTitleWidget %d] MoveIndex Function Call : Up"), __LINE__);
        mCurrentSelectIndex -= 1;

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
    }
    else if (InputType == EWidgetInputType::Down && mMaxSelectIndex > mCurrentSelectIndex)
    {
        UE_LOG(LogTemp, Log, TEXT("[UFQTitleWidget %d] MoveIndex Function Call : Down"), __LINE__);
        mCurrentSelectIndex += 1;

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

void UFQTitleWidget::SelectButton()
{

}

void UFQTitleWidget::CancelButton()
{

}
