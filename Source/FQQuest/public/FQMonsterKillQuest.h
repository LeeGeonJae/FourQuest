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

	// 부모 가상함수
	virtual void Execute() override;
	virtual void UpdateQuest(float DeltaTime) override;

	// 겟셋 함수
	FORCEINLINE void SetQuestMonsterType(const EQuestMonsterType MonsterType) { mMonsterType = MonsterType; }
	FORCEINLINE EQuestMonsterType GetQuestMonsterType() const { return mMonsterType; }

protected:
	// 부모 가상함수
	virtual void BeginPlay() override;

private:
	// 콜백 함수
	void TryUpdateQuestState(EQuestMonsterType MonsterType);

private:
	EQuestMonsterType mMonsterType;
};
