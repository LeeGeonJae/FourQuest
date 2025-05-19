// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "FQGameCore/Player/FQPlayerCharacterInterface.h"
#include "FQPlayer/Public/FQPlayerActionState.h"

#include "FQPlayerBase.generated.h"

UCLASS()
class FQPLAYER_API AFQPlayerBase : public ACharacter, public IFQPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQPlayerBase();
	virtual void Tick(float DeltaSeconds) override;

	// FQPlayerCharacterInterface
	virtual FTransform GetTransform() const override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Knight
	void SetHitReacting(bool HitReacting);

	void ProcessNextSection(const FName& SectionName);

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
	TObjectPtr<class UInputAction> mSwordAttackAction;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mCamera;

	// Effect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> mEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mEffectSystem;

	// Knight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mSwordAttackAnim;

	void StartSwordAttack();
	void EndSwordAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EKnightSwordAttackState mSwordAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EKnightSwordAttackComboState mSwordAttackComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EHitState mHitState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackWaitTime2;

private:
	// Dash
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

	// Setting
	void SetInputMappingContext();
	void SetMovement();

	// Knight
	FTimerHandle mKnightComboTimer;
	FTimerHandle mKnightCoolTimer;

	void ResetCombo();
	void ResetCoolDown();
	void ProcessSwordAttack();
	void PressedSwordAttack();

	// X 버튼을 길게 누르고 있는 상태인지 확인하는 플래그
	uint8 mbIsPressedX : 1;
};
