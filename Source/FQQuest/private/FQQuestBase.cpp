#include "FQQuestBase.h"

#include "FQQuestStartedState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestInProgressState.h"
#include "FQUI\Quest\FQQuestWidget.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
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
}


void AFQQuestBase::UpdateQuest(float DeltaTime)
{
	mCurrentState->UpdateState(DeltaTime);
}

void AFQQuestBase::SetNewState(const EQuestStateType NewState)
{
	switch (NewState)
	{
	case EQuestStateType::InPrograss:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestInProgressState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();
	}
	break;
	case EQuestStateType::Exit:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestCompletedState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();
	}
	break;
	}
}