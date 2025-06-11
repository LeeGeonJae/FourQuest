// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageDataAsset.h"

UFQMageDataAsset::UFQMageDataAsset()
{
	mDefaultSpeed = 200.0f;
	mHealth = 200.0f;

	mExplosionCoolTime = 2.0f;
	mExplosionCircleSpeed = 200.0f;
	mMinDistance = 200.0f;
	mMaxDistance = 500.0f;
	mMinScale = 0.5f;

	mLaserRotationSpeed = 5.0f;
	mLaserDamageTime = 0.1f;
	mLaserDamageAmount = 3.0f;
	mLaserMinDuration = 3.0f;
	mLaserCoolTime = 2.0f;
}
