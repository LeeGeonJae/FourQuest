// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestBase.h"
#include "FQGameCore\Common.h"
#include "FQMonsterKillQuest.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API AFQMonsterKillQuest : public AFQQuestBase
{
	GENERATED_BODY()
	
public:
	AFQMonsterKillQuest();

	virtual void Execute() override;
	virtual void Update(float DeltaTime) override;

	FORCEINLINE void SetMonsterType(const EQuestMonsterType MonsterType) { mMonsterType = MonsterType; }
	FORCEINLINE EQuestMonsterType GetMonsterType() const { return mMonsterType; }
	FORCEINLINE void SetClearMonsterKillNumber(int32 ClearMonsterKillNumber) { mClearMonsterKillNumber = ClearMonsterKillNumber; }
	FORCEINLINE int32 GetClearMonsterKillNumber() { return mClearMonsterKillNumber; }
	FORCEINLINE void SetCurrentMonsterKillNumber(int32 CurrentMonsterKillNumber) { mCurrentMonsterKillNumber = CurrentMonsterKillNumber; }
	FORCEINLINE int32 GetCurrentMonsterKillNumber() { return mCurrentMonsterKillNumber; }
	FORCEINLINE void AddCurrentMonsterKillNumber(int32 AddMonsterKillNumber) { mCurrentMonsterKillNumber += AddMonsterKillNumber; }

protected:
	virtual void BeginPlay() override;

private:
	EQuestMonsterType mMonsterType;

	int32 mClearMonsterKillNumber;
	int32 mCurrentMonsterKillNumber;
};
