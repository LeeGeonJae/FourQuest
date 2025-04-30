// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQSoulDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQSOUL_API UFQSoulDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQSoulDataAsset();

	UPROPERTY(EditAnywhere, Category = Walk)
	float	mWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Dash)
	float	mDashDuration;

	UPROPERTY(EditAnywhere, Category = Dash)
	float	mDashSpeed;
};
