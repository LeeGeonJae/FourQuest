// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FQGameCore\Common.h"
#include "FQ_GameInstance_InGame.generated.h"


UCLASS()
class FOURQUEST_API UFQ_GameInstance_InGame : public UGameInstance
{
	GENERATED_BODY()
	
	using PlayerControllerId = int32;
public:
	UFQ_GameInstance_InGame();

	FORCEINLINE TMap<PlayerControllerId, FQ_LocalMulti::FQLocalMultiPlayerInfomation> GetLocalMultiPlayerInfomation() const { return mLocalMultiPlayerArr; }

	void SavePlayerInfomation(APawn* Player);
	void SavePlayerInfomation(int32 PlayerControllerId, FQ_LocalMulti::FQLocalMultiPlayerInfomation InPlayerInfomation);

private:
	TMap<PlayerControllerId, FQ_LocalMulti::FQLocalMultiPlayerInfomation> mLocalMultiPlayerArr;
};
