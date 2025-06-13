// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore/Common.h"
#include "FQQuestManager.generated.h"

UCLASS()
class FQQUEST_API AFQQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 콜백 함수
	void OnTriggerCallbackFunction(int32 QuestID, EQuestTriggerType QuestTrigeerType);

protected:
	UPROPERTY(EditAnywhere, Category = FQQuestList)
	TMap<int32, class UFQMonsterKillQuestDataAsset*> mMonsterKillQuestDataList;
	UPROPERTY(EditAnywhere, Category = FQQuestList)
	TMap<int32, class UFQInteractionQuestDataAsset*> mInteractionQuestDataList;
	
	UPROPERTY()
	TArray<TObjectPtr<class AFQQuestBase>> mQuestList;
};
