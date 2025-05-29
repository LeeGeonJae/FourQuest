// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerHUDWidget.h"

#include "Components\Image.h"
#include "Components/HorizontalBox.h"
#include "Components/RadialSlider.h"
#include "Blueprint/UserWidget.h"

#include "FQGameCore\Player\FQPlayerControllerInterface.h"


UFQPlayerHUDWidget::UFQPlayerHUDWidget()
    : mElapsedTime(0.f)
    , mbIsSoulBurning(true)
    , mCurrentFrameIndex()
    , mSoulType(ESoulType::Sword)
{
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Blue/"), TEXT("BlueSoul"), 39, mBlueSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Yellow/"), TEXT("YellowSoul"), 39, mYellowSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Green/"), TEXT("GreenSoul"), 39, mGreenSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Red/"), TEXT("RedSoul"), 39, mRedSoulBurningAnimations);
}

void UFQPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    IFQPlayerControllerInterface* OwnerPlayerController = Cast<IFQPlayerControllerInterface>(mOwningActor);
    if (OwnerPlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] Is not Casting IFQPlayerControllerInterface!!"), __LINE__);
        return;
    }

    UpdateArmourSkill(EArmourType::End);
}

void UFQPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

	PlaySoulBurningAnimation(InDeltaTime);
}

void UFQPlayerHUDWidget::PlaySoulBurningAnimation(float DeltaTime)
{
    if (mSoulBurning && mSoulBurning->GetVisibility() == ESlateVisibility::Visible)
    {
        mElapsedTime += DeltaTime;
        if (mElapsedTime < 0.03f)
        {
            return;
        }

        // 애니메이션 플레이
		mElapsedTime = 0.f;
		if (mSoulType == ESoulType::Sword)
		{
            UpdateSoulBurningAnimation(mYellowSoulBurningAnimations);
		}
		else if (mSoulType == ESoulType::Staff)
		{
            UpdateSoulBurningAnimation(mBlueSoulBurningAnimations);
		}
    }
}

void UFQPlayerHUDWidget::UpdateArmourSkill(EArmourType InArmourType)
{
    if (InArmourType == EArmourType::End)
    {
        mSkillBox->SetVisibility(ESlateVisibility::Hidden);
        mArmourType->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        mSkillBox->SetVisibility(ESlateVisibility::Visible);
        mArmourType->SetVisibility(ESlateVisibility::Visible);
        
        UTexture2D* ArmourTypeTexture = mArmourTypeMap[InArmourType];
        UTexture2D* SkillXTexture = mXSkillMap[InArmourType];
        UTexture2D* SkillATexture = mASkillMap[InArmourType];
        UTexture2D* SkillRTexture = mRSkillMap[InArmourType];
        if (SkillXTexture == nullptr || SkillATexture == nullptr || SkillRTexture == nullptr || ArmourTypeTexture == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("[UpdateArmourSkill %d] Skill Or ArmourType Texture Is Null!!"), __LINE__);
            return;
        }

        mArmourType->SetBrushFromTexture(ArmourTypeTexture);
        mSkill_X->SetBrushFromTexture(SkillXTexture);
        mSkill_A->SetBrushFromTexture(SkillATexture);
        mSkill_R->SetBrushFromTexture(SkillRTexture);
    }
}

void UFQPlayerHUDWidget::UpdateSoulGauge(float GaugeValue)
{
    UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHUDWidget %d] GaugeValue : %f"), __LINE__, GaugeValue);
    mSoulGauge->SetValue(GaugeValue);

    if (GaugeValue >= 1.f)
    {
        if (mSoulBurning)
        {
            mSoulBurning->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        mCurrentFrameIndex = 0;
        mElapsedTime = 0.f;

        if (mSoulBurning)
        {
            mSoulBurning->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}



void UFQPlayerHUDWidget::LoadingSoulBurningTexture(FString Path, FString FileName, uint32 TextureSize, TArray<TObjectPtr<UTexture2D>>& SoulTextureContainer)
{
    for (uint32 i = 0; i < TextureSize; i++)
    {
        FString Number = FString::FromInt(i);
        FString AddFileName = FileName + Number;
        FString AssetPath = Path + AddFileName + TEXT(".") + AddFileName + TEXT("\'");

        UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, *AssetPath);
        if (Texture)
        {
            SoulTextureContainer.Emplace(Texture);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] Failed Find Texture Path : %s"), __LINE__, *AssetPath);
        }
    }
}

void UFQPlayerHUDWidget::UpdateSoulBurningAnimation(TArray<UTexture2D*> SoulAnimationKey)
{
    if (SoulAnimationKey.Num() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] SoulAnimationKey Is Empty!!"), __LINE__);
        return;
    }

    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % SoulAnimationKey.Num();
    if (mSoulBurning)
    {
        mSoulBurning->SetBrushFromTexture(SoulAnimationKey[mCurrentFrameIndex]);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mSoulBurning Is Nullptr!!"), __LINE__);
    }
}
