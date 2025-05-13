// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FQGameCore\Common.h"
#include "FQPlayerState_InGame.generated.h"

UCLASS()
class FOURQUEST_API AFQPlayerState_InGame : public APlayerState
{
	GENERATED_BODY()
	
public:
    AFQPlayerState_InGame();

public:
    // Delegate
    FQSoulTypeChange mSoulChangeDelegate;

    // GetSet Funtion
    FORCEINLINE ESoulType GetSoulType() const { return mSoulType; }
    FORCEINLINE void SetSoulType(ESoulType NewType) { mSoulType = NewType; mSoulChangeDelegate.Broadcast(NewType); }

protected:
    // Soul
    UPROPERTY(BlueprintReadOnly, Category = FQSoul)
    ESoulType mSoulType;
};
