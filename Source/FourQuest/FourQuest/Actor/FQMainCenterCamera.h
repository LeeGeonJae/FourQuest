// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQMainCenterCamera.generated.h"

UCLASS()
class FOURQUEST_API AFQMainCenterCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQMainCenterCamera();

	virtual void Tick(float DeltaTime) override;

private:
	void CameraMoveControl(float DeltaTime);
	void CameraZoomControl(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UBoxComponent>> mBoxComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraBlend, Meta = (AllowPrivateAccess = "true"))
	float mCameraBlendValue;
	FVector mCurrentCameraLocation;

	float mCurrentCameraZoom;
	FVector mCameraZoomMin;
	FVector mCameraZoomMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraRange, Meta = (AllowPrivateAccess = "true"))
	FVector2D mBaseRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraRange, Meta = (AllowPrivateAccess = "true"))
	float mRangeMinScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraRange, Meta = (AllowPrivateAccess = "true"))
	float mRangeMaxScale;
};
