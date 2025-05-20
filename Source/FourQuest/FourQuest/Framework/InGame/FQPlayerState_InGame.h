// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FQGameCore\Player\FQPlayerStateInterface.h"
#include "FQPlayerState_InGame.generated.h"

UCLASS()
class FOURQUEST_API AFQPlayerState_InGame : public APlayerState, public IFQPlayerStateInterface
{
	GENERATED_BODY()
	
public:
    AFQPlayerState_InGame();

public:
    // Delegate
    FQSoulTypeChangeDelegate mSoulChangeDelegate;
    FQArmourTypeChangeDelegate mArmourChangeDelegate;
    FQSoulGaugeSettingDelegate mSoulGaugeDelegate;

    // Interface Funtion
    virtual ESoulType GetSoulType() const override { return mSoulType; }
    virtual void SetSoulType(const ESoulType NewType) override { mSoulType = NewType; mSoulChangeDelegate.Broadcast(mSoulType); }
    virtual EArmourType GetArmourType() const override { return mArmourType; }
    virtual void SetArmourType(const EArmourType NewType) override { mArmourType = NewType; mArmourChangeDelegate.Broadcast(mArmourType); }
    virtual void AddSoulGauge(const int32 AddValue) override;
    virtual void SetSoulGauge(const int32 NewValue) override;
    virtual int32 GetSoulGauge() const override { return mCurrentSoulGauge; }

protected:
    // Soul
    UPROPERTY(BlueprintReadOnly, Category = FQSoul)
    ESoulType mSoulType;
    UPROPERTY(BlueprintReadWrite, Category = FQSoul)
    int32 mCurrentSoulGauge;
    int32 mMaxSoulGaue;

    // Armour
    UPROPERTY(BlueprintReadOnly, Category = FQArmour)
    EArmourType mArmourType;
};
