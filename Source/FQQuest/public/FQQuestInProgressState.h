// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestInProgressState.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestInProgressState : public UFQQuestStateBase
{
	GENERATED_BODY()

public:
	UFQQuestInProgressState();

	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
};
