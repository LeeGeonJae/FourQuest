// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQNavigationQuestDataAsset.generated.h"

UCLASS()
class FQQUEST_API UFQNavigationQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQNavigationQuestDataAsset();

	UPROPERTY(EditAnywhere, Category = FQQuestCheck, meta = (ToolTip = "퀘스트 클리어 여부"))
	uint8 mQuestClear : 1;

	UPROPERTY(EditAnywhere, Category = FQQuestText, meta = (ToolTip = "UI에 표시될 퀘스트 내용"))
	FString mDescription;

	//UPROPERTY(EditAnywhere, Category = FQQuestConditions, meta = (ToolTip = "퀘스트 클리어 조건 : 특정 오브젝트 클래스 충돌"))
	//TSubclassOf<class AFQMonsterBase> mTargetObjectClass;
	UPROPERTY(EditAnywhere, Category = FQQuestConditions, meta = (ToolTip = "퀘스트 클리어 조건 : 조건 퀘스트 아이디"))
	TArray<int32> mSubQuestList;
};
