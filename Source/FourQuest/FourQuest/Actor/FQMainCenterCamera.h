// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQMainCenterCamera.generated.h"

UENUM(BlueprintType)
enum class EFrustumDirection : uint8
{
	Top,
	Bottom,
	Left,
	Right
};

UCLASS()
class FOURQUEST_API AFQMainCenterCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQMainCenterCamera();

protected:
	// Parent Class Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void CameraMoveControl(float DeltaTime);
	void CameraZoomControl(float DeltaTime);
	void CameraWallCollisionUpdate(float DeltaTime);

	void RaycastFrustumEdges();

private:
	// Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mFollowCamera;

	// Box Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mTopBoxComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mBottomBoxComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mLeftBoxComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mRightBoxComponents;

	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFQCameraDataAsset> mCameraDataAsset;

	// Common
	FVector mCurrentCameraLocation;
	float mCurrentCameraZoomValue;
};
