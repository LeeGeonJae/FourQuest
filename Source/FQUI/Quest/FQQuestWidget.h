// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQQuestWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQQuestWidget();

	// 일반 함수
	void UpdateQuestState(const EQuestStateType QuestStateType);
	void SetQuestDescriptionText(FString QuestDescriptionText);
	void SetQuestConditionText(FString QuestConditionText);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<EQuestStateType, TObjectPtr<UTexture2D>> mQuestStateTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<EQuestStateType, TObjectPtr<UTexture2D>> mQuestStateCheckBoxTexture;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCurrentStateImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mQuestDescriptionText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mQuestConditionText;
};
