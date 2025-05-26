// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQSoulSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulSelectUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQSoulSelectUI();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mSoulName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mSoulInfomation;
};
