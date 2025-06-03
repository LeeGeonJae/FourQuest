// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerController_InGame.h"
#include "FQPlayerUIActor.generated.h"

UCLASS()
class FOURQUEST_API AFQPlayerUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQPlayerUIActor();

	// Callback Function
	void UpdatePlayerNumber(int32 PlayerControllerNumber, ESoulType PlayerSoulType);
	void UpdatePlayerHp(float HpValue);
	void UpdateArmourGauge(bool bIsVisible, float GaugeValue);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<class UFQWidgetComponent> mPlayerHpWidgetComponent;
	UPROPERTY()
	TObjectPtr<class UFQPlayerHpWidget> mPlayerHpWidget;

	UPROPERTY()
	TObjectPtr<class UFQWidgetComponent> mArmourGaugeWidgetComponent;
	UPROPERTY()
	TObjectPtr<class UFQSoulGaugeWidget> mArmourGaugeWidget;
};
