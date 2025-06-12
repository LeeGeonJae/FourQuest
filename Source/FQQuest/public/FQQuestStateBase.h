// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FQQuestStateBase.generated.h"

UCLASS()
class FQQUEST_API UFQQuestStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFQQuestStateBase() {}

	virtual void Enter() abstract;
	virtual void Update(float DeltaTime) abstract;
	virtual void Exit() abstract;
};
