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
	int8 mQuestClear : 1;

	UPROPERTY(EditAnywhere, Category = FQQuestText, meta = (ToolTip = "UI에 표시될 퀘스트 제목"))
	FString mTitle;
	UPROPERTY(EditAnywhere, Category = FQQuestText, meta = (ToolTip = "UI에 표시될 퀘스트 내용"))
	FString mDescription;
};
