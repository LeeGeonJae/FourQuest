#include "FQQuestBase.h"

#include "FQQuestStartedState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestInProgressState.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
{
}

void AFQQuestBase::BeginPlay()
{
	Super::BeginPlay();

	mCurrentState = CreateDefaultSubobject<UFQQuestStartedState>(TEXT("StartedState"));
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
		mCurrentState = CreateDefaultSubobject<UFQQuestInProgressState>(TEXT("QuestInProgressState"));
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();
	}
	break;
	case EQuestStateType::Exit:
	{
		mCurrentState->ExitState();
		mCurrentState = CreateDefaultSubobject<UFQQuestCompletedState>(TEXT("QuestCompletedState"));
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();
	}
	break;
	}
}