// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonArmour.h"
#include "FQArmourInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFQArmourInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQArmourInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PickArmour() abstract;

	virtual EArmourType GetArmourType() const abstract;
	virtual FTransform GetActorTransform() const abstract;
};
