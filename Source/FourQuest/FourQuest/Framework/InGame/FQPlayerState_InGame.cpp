// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerState_InGame.h"
#include "FQGameMode_InGame.h"
#include "FQUI\Player\FQPlayerHUDWidget.h"

AFQPlayerState_InGame::AFQPlayerState_InGame() 
	: mSoulType(ESoulType::End)
	, mArmourType(EArmourType::End)
	, mMaxSoulGaue(100)
	, mCurrentSoulGauge(0)
{
}

void AFQPlayerState_InGame::AddSoulGauge(const int32 AddValue)
{
	mCurrentSoulGauge += AddValue; 
	if (mCurrentSoulGauge > mMaxSoulGaue)
	{
		mCurrentSoulGauge = mMaxSoulGaue;
	}

	float Value = (float)mCurrentSoulGauge / (float)mMaxSoulGaue;
	mSoulGaugeDelegate.Broadcast(Value);
}

void AFQPlayerState_InGame::SetSoulGauge(const int32 NewValue)
{
	mCurrentSoulGauge = NewValue;
	if (mCurrentSoulGauge > mMaxSoulGaue)
	{
		mCurrentSoulGauge = mMaxSoulGaue;
	}

	float Value = (float)mCurrentSoulGauge / (float)mMaxSoulGaue;
	mSoulGaugeDelegate.Broadcast(Value);
}
