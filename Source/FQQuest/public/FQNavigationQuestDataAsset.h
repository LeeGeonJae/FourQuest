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

	UPROPERTY(EditAnywhere, Category = FQWalk, meta = (ToolTip = "영혼 기본 속도"))
	int32 mCollisionBoxNumber;
};
