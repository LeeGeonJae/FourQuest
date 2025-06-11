// Fill out your copyright notice in the Description page of Project Settings.


#include "FQKnightDataAsset.h"

UFQKnightDataAsset::UFQKnightDataAsset()
{
	mDefaultSpeed = 200.0f;
	mMaxHp = 200.0f;

	mBashSpeed = 500.0f;
	mBashDuration = 0.5f;
	mBashCoolTime = 1.0f;
	mBashStrength = 500.0f;

	mSwordAttackCoolTime = 0.0f;
	mSwordAttackWaitTime1 = 0.0f;
	mSwordAttackWaitTime2 = 0.0f;
	mSwordAttackDamage1 = 5.0f;
	mSwordAttackDamage2 = 10.0f;
	mSwordAttackStrength = 200.0f;

	mShieldWalkRatio = 50.0f;
	mShieldStrength = 500.0f;
	mShieldCoolTime = 0.0f;
	mShieldRotationSpeed = 5.0f;
}
