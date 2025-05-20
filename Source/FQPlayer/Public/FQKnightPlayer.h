// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerBase.h"
#include "FQKnightPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API AFQKnightPlayer : public AFQPlayerBase
{
	GENERATED_BODY()
	
public:
	// Default
	AFQKnightPlayer();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ProcessNextSection(const FName& SectionName) override;
	void EnableSwordAttackVolume();
	void DisableSwordAttackVolume();

protected:
	// Default
	virtual void BeginPlay() override;

	virtual void SetInputMappingContext() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mKnightMappingContext;

	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "대쉬 속도"))
	float mDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float mDashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float mDashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> mDashCurve;

	void Dash();

	// Sword Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mSwordAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mSwordAttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EKnightSwordAttackState mSwordAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EKnightSwordAttackComboState mSwordAttackComboState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	float mSwordAttackWaitTime2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mSwordAttackVolume;

	void StartSwordAttack();
	void EndSwordAttack();

private:
	// Dash
	FVector mDashDirection;

	// 대시할 수 있는 상태인지 확인하는 플래그
	uint8 mbCanDash : 1;
	// 대시를 하는 중인지 확인하는 플래그
	uint8 mbIsDashing : 1;

	FTimerHandle mDashCoolTimer;

	float mDashElapsedTime;

	void StartDash();
	void EndDash();
	void ResetDash();

	// Sword Attack
	FTimerHandle mKnightComboTimer;
	FTimerHandle mKnightCoolTimer;

	void ResetCombo();
	void ResetCoolDown();
	void ProcessSwordAttack();
	void PressedSwordAttack();
};
