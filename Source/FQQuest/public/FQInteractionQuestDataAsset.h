// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQGameCore\Common.h"
#include "FQInteractionQuestDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQInteractionQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQInteractionQuestDataAsset();

	UPROPERTY(EditAnywhere, Category = FQQuestCheck, meta = (ToolTip = "퀘스트 클리어 여부"))
	uint8 mQuestClear : 1;

	UPROPERTY(EditAnywhere, Category = FQQuestText, meta = (ToolTip = "UI에 표시될 퀘스트 내용"))
	FString mDescription;

	UPROPERTY(EditAnywhere, Category = FQQuestConditions, meta = (ToolTip = "퀘스트 클리어 조건 : 상호작용 오브젝트 타입"))
	EQuestInteractionType mInteractionType;
	UPROPERTY(EditAnywhere, Category = FQQuestConditions, meta = (ToolTip = "퀘스트 클리어 조건 : 조건 퀘스트 아이디"))
	TArray<int32> mSubQuestList;

	UPROPERTY(EditAnywhere, Category = FQQuestClear, meta = (ToolTip = "퀘스트 클리어 : 다음 퀘스트 생성"))
	TArray<int32> mNextQuestList;
};
