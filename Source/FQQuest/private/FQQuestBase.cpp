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

	//mCurrentState = CreateDefaultSubobject<UFQQuestStartedState>(TEXT("StartedState"));
	//mCurrentState->SetOwnerQuestObject(this);
	//mCurrentState->Enter();
}


void AFQQuestBase::Update(float DeltaTime)
{
	//mCurrentState->Update(DeltaTime);
}

void AFQQuestBase::SetNewState(UFQQuestStateBase* NewState)
{
	//mCurrentState->Exit();
	//mCurrentState = NewState;
	//mCurrentState->SetOwnerQuestObject(this);
	//mCurrentState->Enter();
}