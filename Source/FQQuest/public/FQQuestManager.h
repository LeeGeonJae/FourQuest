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

	// 일반 함수
	void CreateQuest(int32 QuestID);

protected:
	// 퀘스트
	UPROPERTY()
	TMap<int32, TObjectPtr<class AFQQuestBase>> mQuestList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQUI)
	TSubclassOf<class UFQQuestListUI> mQuestListUIClass;
	UPROPERTY()
	TObjectPtr<class UFQQuestListUI> mQuestListUI;
};
