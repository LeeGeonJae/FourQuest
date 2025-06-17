#include "FQInteractionQuest.h"

#include "FQGameCore\Quest/FQQuestSystem.h"

AFQInteractionQuest::AFQInteractionQuest()
    : mInteractionType(EQuestInteractionType::None)
{
}

void AFQInteractionQuest::BeginPlay()
{
	Super::BeginPlay();

	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		QuestSystem->mInteractionDelegate.AddUObject(this, &AFQInteractionQuest::TryUpdateQuestState);
	}
}

void AFQInteractionQuest::TryUpdateQuestState(EQuestInteractionType InteractionType)
{
    if (mInteractionType == InteractionType)
    {
		UpdateQuestCondition(1);
    }
}


void AFQInteractionQuest::Execute()
{


}

void AFQInteractionQuest::UpdateQuest(float DeltaTime)
{
	Super::UpdateQuest(DeltaTime);


}