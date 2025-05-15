// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerHUDWidget.h"
#include "Components\Image.h"
#include "Blueprint/UserWidget.h"
#include "FQGameCore\Controller\FQPlayerControllerInterface.h"

UFQPlayerHUDWidget::UFQPlayerHUDWidget()
    : mKnightSoulTexture(nullptr)
    , mMagicSoulTexture(nullptr)
{
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
}

void UFQPlayerHUDWidget::UpdateSoulIcon(ESoulType InSoulType)
{
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