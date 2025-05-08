// Fill out your copyright notice in the Description page of Project Settings.


#include "FQArmourWidget.h"
#include "Components\Image.h"
#include "Blueprint/UserWidget.h"
#include "FQGameCore\Armour\FQArmourInterface.h"

UFQArmourWidget::UFQArmourWidget()
{



}

void UFQArmourWidget::NativeConstruct()
{
    Super::NativeConstruct();
    IFQArmourInterface* OwnerArmour = Cast<IFQArmourInterface>(mOwningActor);

    if (!OwnerArmour)
    {
        return;
    }

    EArmourType OwnerArmourType = OwnerArmour->GetArmourType();
    UTexture2D* SelectedTexture = nullptr;
    switch (OwnerArmourType)
    {
    case EArmourType::Knight:
        SelectedTexture = mKnightArmourTexture;
        break;
    case EArmourType::Magic:
        SelectedTexture = mMagicArmourTexture;
        break;
    }

    UImage* ArmourTypeImage = Cast<UImage>(GetWidgetFromName(TEXT("ArmourType")));
    ArmourTypeImage->SetBrushFromTexture(SelectedTexture);
}
