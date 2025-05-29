// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQKnightDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQKnightDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQKnightDataAsset();

	// Default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "기본 이동 속도"))
	float mDefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "체력"))
	float mHealth;

	// Bash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_A, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 속도"))
	float mBashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_A, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 시간"))
	float mBashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_A, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 쿨타임"))
	float mBashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_A, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 밀어내는 힘"))
	float mBashStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_A, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 속도감 그래프"))
	TObjectPtr<class UCurveFloat> mBashCurve;
	
	// Sword Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 쿨타임"))
	float mSwordAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 1타 -> 2타 입력 대기 시간"))
	float mSwordAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 2타 -> 3타 입력 대기 시간"))
	float mSwordAttackWaitTime2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 1타 데미지"))
	float mSwordAttackDamage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 2타 데미지"))
	float mSwordAttackDamage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 3타 밀어내는 힘"))
	float mSwordAttackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mSwordAttackableTypes;

	// Shield
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[R] 이동 속도 감소량"))
	float mShieldWalkRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_R, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[R] 밀어내는 힘"))
	float mShieldStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_R, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[R] 쿨타임"))
	float mShieldCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill_R, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[R] 회전 속도"))
	float mShieldRotationSpeed;
};
