// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerHUDWidget.h"
#include "Components\Image.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"
#include "FQGameCore\Player\FQPlayerControllerInterface.h"


UFQPlayerHUDWidget::UFQPlayerHUDWidget()
    : mKnightSoulTexture(nullptr)
    , mMagicSoulTexture(nullptr)
    , mElapsedTime(0.f)
    , mbIsSoulBurning(true)
    , mCurrentFrameIndex()
    , mSoulType(ESoulType::Knight)
{
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Blue/"), TEXT("BlueSoul"), 39, mBlueSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Yellow/"), TEXT("YellowSoul"), 39, mYellowSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Green/"), TEXT("GreenSoul"), 39, mGreenSoulBurningAnimations);
    LoadingSoulBurningTexture(TEXT("/Script/Engine.Texture2D'/Game/Textures/ui/Player_HUD_Sprite/Red/"), TEXT("RedSoul"), 39, mRedSoulBurningAnimations);
}

void UFQPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (mKnightSoulTexture == nullptr || mMagicSoulTexture == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] KnightArmour Or MagicArmour Texture Is Null!!"), __LINE__);
        return;
    }

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
    if (mbIsSoulBurning)
    {
        mElapsedTime += DeltaTime;
        if (mElapsedTime < 0.03f)
        {
            return;
        }

        // 애니메이션 플레이
		mElapsedTime = 0.f;
		if (mSoulType == ESoulType::Knight)
		{
            if (mYellowSoulBurningAnimations.Num() <= 0)
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mYellowSoulBurningAnimations Is Empty!!"), __LINE__);
                return;
            }

			mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mYellowSoulBurningAnimations.Num();
            if (mSoulBurning)
            {
                mSoulBurning->SetBrushFromTexture(mYellowSoulBurningAnimations[mCurrentFrameIndex]);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mSoulBurning Is Nullptr!!"), __LINE__);
            }
		}
		else if (mSoulType == ESoulType::Magic)
		{
            if (mBlueSoulBurningAnimations.Num() <= 0)
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mBlueSoulBurningAnimations Is Empty!!"), __LINE__);
                return;
            }

			mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mBlueSoulBurningAnimations.Num();
            if (mBlueSoulBurningAnimations.Num() > 0 && mSoulBurning)
            {
                mSoulBurning->SetBrushFromTexture(mBlueSoulBurningAnimations[mCurrentFrameIndex]);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mSoulBurning Is Nullptr!!"), __LINE__);
            }
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


void UFQPlayerHUDWidget::UpdateSoulIcon(ESoulType InSoulType)
{
    mSoulType = InSoulType;
    UTexture2D* SelectedTexture = nullptr;
    switch (InSoulType)
    {
    case ESoulType::Knight:
        SelectedTexture = mKnightSoulTexture;
        UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHUDWidget %d] PlayerHUDWidget Update KnightSoulType Icon"), __LINE__);
        break;
    case ESoulType::Magic:
        SelectedTexture = mMagicSoulTexture;
        UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHUDWidget %d] PlayerHUDWidget Update MagicSoulType Icon"), __LINE__);
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] SoulType Is End"), __LINE__);
        break;
    }

    UImage* SoulTypeImage = Cast<UImage>(GetWidgetFromName(TEXT("SoulType")));
    if (SoulTypeImage && SelectedTexture != nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHUDWidget %d] Changed ArmourType Iamage"), __LINE__);
        SoulTypeImage->SetBrushFromTexture(SelectedTexture);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] PlayerHUD have not \'SoulType\' Widget Component!!"), __LINE__);
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
        
        UTexture2D* ArmourType = mArmourTypeMap[InArmourType];
        UTexture2D* SkillX = mXSkillMap[InArmourType];
        UTexture2D* SkillA = mASkillMap[InArmourType];
        UTexture2D* SkillR = mRSkillMap[InArmourType];
        if (SkillX == nullptr || SkillA == nullptr || SkillR == nullptr || ArmourType == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("[UpdateArmourSkill %d] Skill Texture Is Null!!"), __LINE__);
            return;
        }

        mArmourType->SetBrushFromTexture(ArmourType);
        mSkill_X->SetBrushFromTexture(SkillX);
        mSkill_A->SetBrushFromTexture(SkillA);
        mSkill_R->SetBrushFromTexture(SkillR);
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