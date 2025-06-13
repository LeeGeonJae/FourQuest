// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FQGameCore\Common.h"
#include "FQMonsterKillQuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FQGAMECORE_API UFQMonsterKillQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFQMonsterKillQuestComponent();

	FQ_QuestDelegate::FQQuestMonsterDelegate mMonsterQuestDelegate;

	// 퀘스트 호출
	void ExecuteQuest() { mMonsterQuestDelegate.ExecuteIfBound(mMonsterType); }

	// 몬스터 타입 지정
	FORCEINLINE void SetMonsterType(const EQuestMonsterType MonsterType) { mMonsterType = MonsterType; }
	FORCEINLINE EQuestMonsterType GetMonsterType() const { return mMonsterType; }
	
private:
	EQuestMonsterType mMonsterType;
};
