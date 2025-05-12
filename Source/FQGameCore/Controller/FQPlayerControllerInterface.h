// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore\Soul\CommonSoul.h"
#include "FQPlayerControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQPlayerControllerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SetSoulType(ESoulType InSoulType) abstract;
	UFUNCTION()
	virtual ESoulType GetSoulType() abstract;
};
