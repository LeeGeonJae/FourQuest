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
	void UpdateQuestStateAnimation(float Value);
	void UpdateQuestStateCheckBoxAnimation(float Value);
	void UpdateQuestCondition(const int32 CurrentConditionNumber, const int32 ClearConditionNumber);
	void SetQuestDescriptionText(FString QuestDescriptionText);
	void SetQuestConditionText(FString QuestConditionText);

protected:
	// 부모 함수
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<EQuestStateType, TObjectPtr<UTexture2D>> mQuestStateTexture;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCurrentStateImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckLeft;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckRight;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mQuestDescriptionText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mQuestConditionText;
};
