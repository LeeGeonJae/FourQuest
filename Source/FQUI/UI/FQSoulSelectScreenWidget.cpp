// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulSelectScreenWidget.h"

#include "Components/Image.h"

#include "FQSoulComplateUI.h"
#include "FQSoulSelectUI.h"

UFQSoulSelectScreenWidget::UFQSoulSelectScreenWidget()
{
}

void UFQSoulSelectScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

    constexpr int32 NumWidgets = 4; // 원하는 개수만큼 반복
    for (int32 i = 1; i <= NumWidgets; ++i)
    {
        // NoSelect Image
        FString NoSelectName = FString::Printf(TEXT("P%d_NoSelect"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*NoSelectName)))
        {
            if (UImage* Img = Cast<UImage>(FoundWidget))
            {
                mNoSelectSoulArr.Add(Img);
            }
        }

        // Soul Select UI
        FString SelectUIName = FString::Printf(TEXT("P%d_SelectCheck"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*SelectUIName)))
        {
            if (UFQSoulSelectUI* SelectUI = Cast<UFQSoulSelectUI>(FoundWidget))
            {
                mSoulSelectUIArr.Add(SelectUI);
            }
        }

        // Soul Complete UI
        FString ComplateUIName = FString::Printf(TEXT("P%d_ComplateSoul"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*ComplateUIName)))
        {
            if (UFQSoulComplateUI* ComplateUI = Cast<UFQSoulComplateUI>(FoundWidget))
            {
                mSoulComplateUIArr.Add(ComplateUI);
            }
        }
    }
}

void UFQSoulSelectScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFQSoulSelectScreenWidget::WidgetInput(EWidgetInputType InputType, int32 ControllerId)
{
    switch (InputType)
    {
    case EWidgetInputType::Left:
        MoveIndex(EWidgetInputType::Left, ControllerId);
        break;
    case EWidgetInputType::Right:
        MoveIndex(EWidgetInputType::Right, ControllerId);
        break;
    case EWidgetInputType::Up:
        MoveIndex(EWidgetInputType::Up, ControllerId);
        break;
    case EWidgetInputType::Down:
        MoveIndex(EWidgetInputType::Down, ControllerId);
        break;
    case EWidgetInputType::Select:
        SelectButton(ControllerId);
        break;
    case EWidgetInputType::Cancel:
        CancelButton(ControllerId);
        break;
    }
}

void UFQSoulSelectScreenWidget::MoveIndex(EWidgetInputType InputType, int32 ControllerId)
{

}

void UFQSoulSelectScreenWidget::SelectButton(int32 ControllerId)
{

}

void UFQSoulSelectScreenWidget::CancelButton(int32 ControllerId)
{

}
