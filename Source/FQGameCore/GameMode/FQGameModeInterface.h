// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "FQGameModeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQGameModeInterface
{
	GENERATED_BODY()

public:
	// Input Funtion
	virtual void MoveButton(const FInputActionValue& Value) abstract;
	virtual void CancelInteraction() abstract;
	virtual void SelectInteraction() abstract;

};
