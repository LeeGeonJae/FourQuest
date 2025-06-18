#include "FQQuestWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

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
		float BrushSize = FMath::Lerp(30, 15, Value);
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

void UFQQuestWidget::AddSubQuestListWidget(UFQUserWidget* QuestWidget)
{
	if (!QuestWidget || !mSubQuestListBox)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestListUI %d] QuestWidget Or mQuestListBox가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	// VerticalBox에 위젯 추가
	UVerticalBoxSlot* NewSlot = mSubQuestListBox->AddChildToVerticalBox(QuestWidget);
	if (NewSlot)
	{
		// 슬롯 속성 설정
		NewSlot->SetPadding(FMargin(0.f, 10.f, 0.f, 10.f));
		NewSlot->SetHorizontalAlignment(HAlign_Left);
		NewSlot->SetVerticalAlignment(VAlign_Top);
	}
}

void UFQQuestWidget::RemoveSubQuestWidget(UFQUserWidget* QuestWidget)
{
	if (mSubQuestListBox && QuestWidget)
	{
		mSubQuestListBox->RemoveChild(QuestWidget);
	}
}

void UFQQuestWidget::RemoveSubQuestWidget()
{
	mSubQuestListBox->ClearChildren();
}
