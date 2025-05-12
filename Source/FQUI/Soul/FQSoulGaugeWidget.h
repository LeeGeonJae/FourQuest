// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI\FQUserWidget.h"
#include "FQSoulGaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulGaugeWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQSoulGaugeWidget();

	void SetChargeGaugeValueSet(float InValue);

protected:
	virtual void NativeConstruct() override;

protected:
	// Widget 게이지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gauge")
	class URadialSlider* mArmourChargeGauge;
};