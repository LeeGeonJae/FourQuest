// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerHUDWidget.h"
#include "Components\Image.h"
#include "Blueprint/UserWidget.h"
#include "FQGameCore\Controller\FQPlayerControllerInterface.h"

UFQPlayerHUDWidget::UFQPlayerHUDWidget()
{
}

void UFQPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("Changed ArmourType Iamage"));

    if (mKnightSoulTexture == nullptr || mMagicSoulTexture == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("KnightArmour Or MagicArmour Texture Is Null!!"));
        return;
    }

    UTexture2D* SelectedTexture = nullptr;
    IFQPlayerControllerInterface* OwnerPlayerController = Cast<IFQPlayerControllerInterface>(mOwningActor);
    if (OwnerPlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Is not Casting IFQPlayerControllerInterface"));
        return;
    }

    ESoulType OwnerArmourType = OwnerPlayerController->GetSoulType();
    switch (OwnerArmourType)
    {
        case ESoulType::Knight:
        SelectedTexture = mKnightSoulTexture;
        UE_LOG(LogTemp, Log, TEXT("PlayerController SoulType Is Knight"));
        break;
		case ESoulType::Magic:
        SelectedTexture = mMagicSoulTexture;
        UE_LOG(LogTemp, Log, TEXT("PlayerController SoulType Is Magic"));
        break;
    }

    UImage* SoulTypeImage = Cast<UImage>(GetWidgetFromName(TEXT("SoulType")));
    if (SoulTypeImage && SelectedTexture != nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("Changed ArmourType Iamage"));
        SoulTypeImage->SetBrushFromTexture(SelectedTexture);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerHUD have not \'SoulType\' Widget Component!!"));
    }
}
