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

    // GetSet Funtion
    virtual ESoulType GetSoulType() const override { return mSoulType; }
    virtual void SetSoulType(ESoulType NewType) override { mSoulType = NewType; mSoulChangeDelegate.Broadcast(NewType); }
    virtual EArmourType GetArmourType() const override { return mArmourType; }
    virtual void SetArmourType(EArmourType NewType) override { mArmourType = NewType; mArmourChangeDelegate.Broadcast(NewType); }

protected:
    // Soul
    UPROPERTY(BlueprintReadOnly, Category = FQSoul)
    ESoulType mSoulType;

    // Armour
    UPROPERTY(BlueprintReadOnly, Category = FQArmour)
    EArmourType mArmourType;
};
