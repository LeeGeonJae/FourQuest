// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestCompletedState.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestCompletedState : public UFQQuestStateBase
{
	GENERATED_BODY()

public:
	UFQQuestCompletedState();

	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
};
