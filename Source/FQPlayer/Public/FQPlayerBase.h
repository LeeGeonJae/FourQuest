// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FQGameCore/Player/FQPlayerCharacterInterface.h"
#include "InputActionValue.h"

#include "FQPlayerBase.generated.h"

UCLASS()
class FQPLAYER_API AFQPlayerBase : public ACharacter, public IFQPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQPlayerBase();

	// FQPlayerCharacterInterface
	virtual FTransform GetTransform() const override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mInteractiveAction;

	void Move(const FInputActionValue& Value);
	void Dash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "기본 속도"))
	float mDefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "대쉬 속도"))
	float mDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float mDashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float mDashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> mDashCurve;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mCamera;

	// Effect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> mEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mEffectSystem;

private:
	FVector mDashDirection;

	// 대시할 수 있는 상태인지 확인하는 플래그
	uint8 mbCanDash : 1;
	// 대시를 하는 중인지 확인하는 플래그
	uint8 mbIsDashing : 1;

	FTimerHandle mDashCoolTimer;

	void StartDash();
	void EndDash();
	void ResetDash();

	float mDashElapsedTime;

	void SetInputMappingContext();
	void SetMovement();
};
