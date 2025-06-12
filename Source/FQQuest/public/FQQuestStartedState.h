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

	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

private:
	float mComplatedTime;
	float mDurationTime;
};
