#include "FQQuestBase.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
{
}

void AFQQuestBase::BeginPlay()
{
	Super::BeginPlay();

	mCurrentState = CreateDefaultSubobject<UFQQuestStartedState>(TEXT("StartedState"));
	mCurrentState->Enter();
}


void AFQQuestBase::Update(float DeltaTime)
{
	mCurrentState->Update(DeltaTime);
}

void AFQQuestBase::SetNewState(UFQQuestStateBase* NewState)
{
	mCurrentState->Exit();
	mCurrentState = std::move(NewState);
	mCurrentState->Enter();
}