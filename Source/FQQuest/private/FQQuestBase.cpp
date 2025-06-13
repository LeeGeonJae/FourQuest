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
	mCurrentState->StateEnter();
}


void AFQQuestBase::Update(float DeltaTime)
{
	mCurrentState->StateUpdate(DeltaTime);
}

void AFQQuestBase::SetNewState(UFQQuestStateBase* NewState)
{
	mCurrentState->StateExit();
	mCurrentState = NewState;
	mCurrentState->SetOwnerQuestObject(this);
	mCurrentState->StateEnter();
}