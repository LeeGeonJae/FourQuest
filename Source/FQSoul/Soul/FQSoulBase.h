// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "InputActionValue.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	AFQSoulBase();

	// Character
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IFQSoulCharacterInterface
	virtual FTransform GetTransform() const override;


private:
	void SetCharacterControl();

	void Move(const FInputActionValue& Value);
	void ChangeArmour();
	void StartDash();

protected:
	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mFollowCamera;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mPickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UFQSoulDataAsset> mSoulDataAsset;


private:
	// 대시 변수
	bool mbIsDashing;
	FVector mDashDirection;
	float	mDashTimer;
};
