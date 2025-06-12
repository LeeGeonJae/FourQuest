// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestStartedState.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestStartedState : public UFQQuestStateBase
{
	GENERATED_BODY()
	
public:
	UFQQuestStartedState();

	virtual void StateEnter() override;
	virtual void StateUpdate(float DeltaTime) override;
	virtual void StateExit() override;

private:
	float mComplatedTime;
	float mDurationTime;
};
