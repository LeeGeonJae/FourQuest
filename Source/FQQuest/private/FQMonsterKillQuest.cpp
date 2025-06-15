#include "FQMonsterKillQuest.h"

#include "FQGameCore\Quest/FQQuestSystem.h"

AFQMonsterKillQuest::AFQMonsterKillQuest()
    : mMonsterType(EQuestMonsterType::None)
    , mCurrentMonsterKillNumber()
    , mClearMonsterKillNumber()
{
}

void AFQMonsterKillQuest::BeginPlay()
{
	Super::BeginPlay();

	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		QuestSystem->mMonsterQuestDelegate.AddUObject(this, &AFQMonsterKillQuest::TryUpdateQuestState);
	}
}

void AFQMonsterKillQuest::TryUpdateQuestState(EQuestMonsterType MonsterType)
{
    if (mMonsterType == MonsterType)
    {
        mCurrentMonsterKillNumber++;
    }
}

void AFQMonsterKillQuest::Execute()
{
}

void AFQMonsterKillQuest::UpdateQuest(float DeltaTime)
{
	Super::UpdateQuest(DeltaTime);


}
