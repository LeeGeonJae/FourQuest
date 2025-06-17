#include "FQQuestBase.h"

#include "FQQuestStartedState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestInProgressState.h"
#include "FQUI\Quest\FQQuestWidget.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
	, mCurrentStateType(EQuestStateType::Started)
	, mQuestCurrentConditionNumber()
	, mQuestClearConditionNumber()
{
	ConstructorHelpers::FClassFinder<UUserWidget> QuestWidgetRef(TEXT("/Game/Blueprints/Quest/WBP_QuestWidget.WBP_QuestWidget_C"));
	if (QuestWidgetRef.Class)
	{
		mQuestWidget = CreateWidget<UFQQuestWidget>(GetWorld(), QuestWidgetRef.Class);
	}
}

void AFQQuestBase::BeginPlay()
{
	Super::BeginPlay();

	// UI
	mQuestWidget->SetQuestDescriptionText(mDescription);

	// State
	mCurrentState = NewObject<UFQQuestStartedState>(this);
	mCurrentState->SetOwnerQuestObject(this);
	mCurrentState->EnterState();
	UpdateQuestCondition(0);
}


void AFQQuestBase::UpdateQuest(float DeltaTime)
{
	mCurrentState->UpdateState(DeltaTime);

	if (!mQuestWidget)
	{
		return;
	}

	switch (mCurrentStateType)
	{
	case EQuestStateType::Started:
	{
		UFQQuestStartedState* MyState = Cast<UFQQuestStartedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
		}
	}
	break;
	case EQuestStateType::InPrograss:
		break;
	case EQuestStateType::Exit:
	{
		UFQQuestCompletedState* MyState = Cast<UFQQuestCompletedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
			mQuestWidget->UpdateQuestStateCheckBoxAnimation(MyState->GetDurationValue());
		}
	}
	break;
	}
}

void AFQQuestBase::SetNewState(const EQuestStateType NewState)
{
	mCurrentStateType = NewState;
	switch (mCurrentStateType)
	{
	case EQuestStateType::InPrograss:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestInProgressState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : InPrograss"), __LINE__);
	}
	break;
	case EQuestStateType::Exit:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestCompletedState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : Exit"), __LINE__);
	}
	break;
	case EQuestStateType::End:
	{
		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : End"), __LINE__);
	}
	break;
	}

	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestState(mCurrentStateType);
	}
}

void AFQQuestBase::UpdateQuestCondition(int32 AddConditionNumber)
{
	mQuestCurrentConditionNumber += AddConditionNumber;
	if (mQuestCurrentConditionNumber >= mQuestClearConditionNumber)
	{
		SetNewState(EQuestStateType::Exit);
	}

	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestCondition(mQuestClearConditionNumber > mQuestCurrentConditionNumber ? mQuestCurrentConditionNumber : mQuestClearConditionNumber, mQuestClearConditionNumber);
	}
}
