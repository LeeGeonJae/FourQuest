#include "FQQuestWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

UFQQuestWidget::UFQQuestWidget()
{
}

void UFQQuestWidget::NativeConstruct()
{
	if (!mCheckLeft || !mCheckRight)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestWidget %d] mCheckLeft Or mCheckRight가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	mCheckLeft->SetVisibility(ESlateVisibility::Hidden);
	mCheckRight->SetVisibility(ESlateVisibility::Hidden);
}

void UFQQuestWidget::UpdateQuestState(EQuestStateType QuestStateType)
{
	if (QuestStateType == EQuestStateType::End)
	{
		return;
	}

	if (auto FoundTexture = mQuestStateTexture.Find(QuestStateType))
	{
		if (mCurrentStateImage)
		{
			mCurrentStateImage->SetBrushFromTexture(*FoundTexture);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestWidget] QuestStateTexture에 해당 상태(%d)가 존재하지 않습니다!"), (int32)QuestStateType);
	}

	if (QuestStateType == EQuestStateType::Exit)
	{
		if (!mCheckLeft || !mCheckRight)
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQQuestWidget %d] mCheckLeft Or mCheckRight가 유효하지 않습니다!!"), __LINE__);
			return;
		}

		mCheckLeft->SetVisibility(ESlateVisibility::Visible);
		mCheckRight->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFQQuestWidget::UpdateQuestStateAnimation(float Value)
{
	if (mCurrentStateImage)
	{
		UE_LOG(LogTemp, Log, TEXT("[UFQQuestWidget %d] UpdateQuestStateAnimation Value : %f"), __LINE__, Value);
		mCurrentStateImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 1 - Value));
	}
}

void UFQQuestWidget::UpdateQuestStateCheckBoxAnimation(float Value)
{
	if (mCheckLeft && mCheckRight)
	{
		UE_LOG(LogTemp, Log, TEXT("[UFQQuestWidget %d] UpdateQuestStateCheckBoxAnimation Value : %f"), __LINE__, Value);
		float BrushSize = FMath::Lerp(100, 15, Value);
		mCheckLeft->SetBrushSize(FVector2D(BrushSize));
		mCheckRight->SetBrushSize(FVector2D(BrushSize));
	}
}

void UFQQuestWidget::UpdateQuestCondition(const int32 CurrentConditionNumber, const int32 ClearConditionNumber)
{
	if (mQuestConditionText)
	{
		FString NewString = FString::Printf(TEXT("%d / %d"), CurrentConditionNumber, ClearConditionNumber);
		mQuestConditionText->SetText(FText::FromString(NewString));
	}
}

void UFQQuestWidget::SetQuestDescriptionText(FString QuestDescriptionText)
{
	if (mQuestDescriptionText)
	{
		mQuestDescriptionText->SetText(FText::FromString(QuestDescriptionText));
	}
}

void UFQQuestWidget::SetQuestConditionText(FString QuestConditionText)
{
	if (mQuestConditionText)
	{
		mQuestConditionText->SetText(FText::FromString(QuestConditionText));
	}
}
