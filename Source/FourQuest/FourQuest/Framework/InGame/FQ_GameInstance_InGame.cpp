// Fill out your copyright notice in the Description page of Project Settings.

#include "FQ_GameInstance_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FQPlayerController_InGame.h"

UFQ_GameInstance_InGame::UFQ_GameInstance_InGame()
{
	FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation;
	PlayerInfomation.bSpawnLocalPlayer = true;
	PlayerInfomation.mSoulType = ESoulType::Sword;
	PlayerInfomation.mArmourType = EArmourType::End;
	mLocalMultiPlayerArr.Add(0, PlayerInfomation);
}

void UFQ_GameInstance_InGame::SavePlayerInfomation(APawn* Player)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] Player is nullptr!!"), __LINE__);
		return;
	}

	AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(Player->GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] AFQPlayerController_InGame is nullptr!!"), __LINE__);
		return;
	}

	AFQPlayerState_InGame* PlayerState = Cast<AFQPlayerState_InGame>(Player->GetPlayerState());
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] AFQPlayerState_InGame is nullptr!!"), __LINE__);
		return;
	}

	UWorld* World = Player->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] World is nullptr!!"), __LINE__);
		return;
	}

	int32 PlayerControllerId = 0;
	for (ULocalPlayer* LP : LocalPlayers)
	{
		if (LP && LP->PlayerController == PC)
		{
			PlayerControllerId = LP->GetControllerId();
			break;
		}
	}

	if (mLocalMultiPlayerArr.Find(PlayerControllerId) == nullptr)
	{
		FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation;
		PlayerInfomation.mSoulType = PlayerState->GetSoulType();
		PlayerInfomation.mArmourType = PlayerState->GetArmourType();
		mLocalMultiPlayerArr.Add(PlayerControllerId, PlayerInfomation);
	}
	else
	{
		FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation = mLocalMultiPlayerArr[PlayerControllerId];
		PlayerInfomation.mSoulType = PlayerState->GetSoulType();
		PlayerInfomation.mArmourType = PlayerState->GetArmourType();
		mLocalMultiPlayerArr[PlayerControllerId] = PlayerInfomation;
	}
}

void UFQ_GameInstance_InGame::SavePlayerInfomation(int32 PlayerControllerId, FQ_LocalMulti::FQLocalMultiPlayerInfomation InPlayerInfomation)
{
	if (mLocalMultiPlayerArr.Find(PlayerControllerId) == nullptr)
	{
		FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation;
		PlayerInfomation.bSpawnLocalPlayer = InPlayerInfomation.bSpawnLocalPlayer;
		PlayerInfomation.mSoulType = InPlayerInfomation.mSoulType;
		PlayerInfomation.mArmourType = InPlayerInfomation.mArmourType;
		mLocalMultiPlayerArr.Add(PlayerControllerId, PlayerInfomation);
	}
	else
	{
		FQ_LocalMulti::FQLocalMultiPlayerInfomation PlayerInfomation = mLocalMultiPlayerArr[PlayerControllerId];
		PlayerInfomation.bSpawnLocalPlayer = InPlayerInfomation.bSpawnLocalPlayer;
		PlayerInfomation.mSoulType = InPlayerInfomation.mSoulType;
		PlayerInfomation.mArmourType = InPlayerInfomation.mArmourType;
		mLocalMultiPlayerArr[PlayerControllerId] = PlayerInfomation;
	}
}
