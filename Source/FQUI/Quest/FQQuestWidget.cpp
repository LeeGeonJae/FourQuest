#include "FQQuestWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

UFQQuestWidget::UFQQuestWidget()
{
}

void UFQQuestWidget::UpdateQuestState(EQuestStateType QuestStateType)
{
	UTexture2D* QuestState = mQuestStateTexture[QuestStateType];
	UTexture2D* QuestCheckBox = mQuestStateCheckBoxTexture[QuestStateType];
	if (!QuestState || !QuestCheckBox)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestWidget %d] QuestState Or QuestCheckBox가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	mCurrentStateImage->SetBrushFromTexture(QuestState);
	mCheckBox->SetBrushFromTexture(QuestCheckBox);
}

void UFQQuestWidget::SetQuestDescriptionText(FString QuestDescriptionText)
{
	mQuestDescriptionText->SetText(FText::FromString(QuestDescriptionText));
}

void UFQQuestWidget::SetQuestConditionText(FString QuestConditionText)
{
	mQuestConditionText->SetText(FText::FromString(QuestConditionText));
}