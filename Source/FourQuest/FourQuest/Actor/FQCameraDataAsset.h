// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQCameraDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API UFQCameraDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQCameraDataAsset();

	UPROPERTY(EditAnywhere, Category = CameraMove)
	float mCameraBlendValue;

	UPROPERTY(EditAnywhere, Category = CameraZoom)
	float mCameraZoomMin;
	UPROPERTY(EditAnywhere, Category = CameraZoom)
	float mCameraZoomMax;
	UPROPERTY(EditAnywhere, Category = CameraZoom)
	float CameraZoomSpeed;

	UPROPERTY(EditAnywhere, Category = CameraZoomRange)
	FVector2D mBaseCheckRange;
	UPROPERTY(EditAnywhere, Category = CameraZoomRange)
	float mCheckRangeMinScale;
	UPROPERTY(EditAnywhere, Category = CameraZoomRange)
	float mCheckRangeMaxScale;
};
