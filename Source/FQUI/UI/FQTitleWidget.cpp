// Fill out your copyright notice in the Description page of Project Settings.


#include "FQTitleWidget.h"

#include "Components\Image.h"


UFQTitleWidget::UFQTitleWidget()
    : mElapsedTime(0.f)
    , mCurrentFrameIndex(0)
    , mCurrentSelectIndex(0)
    , mSoulAnimations{}
    , mSelectButton()
    , mCurrentSelect()
{
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
