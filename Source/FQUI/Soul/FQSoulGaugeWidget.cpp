// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulGaugeWidget.h"

#include "Components\RadialSlider.h"

UFQSoulGaugeWidget::UFQSoulGaugeWidget()
{
}

void UFQSoulGaugeWidget::SetChargeGaugeValueSet(float InValue)
{
	if (mArmourChargeGauge)
	{
		mArmourChargeGauge->SetValue(InValue);
	}
}

void UFQSoulGaugeWidget::NativeConstruct()
{
	mArmourChargeGauge = Cast<URadialSlider>(GetWidgetFromName(TEXT("ChargeGauge")));
}
