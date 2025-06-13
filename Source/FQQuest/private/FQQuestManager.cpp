#include "FQQuestManager.h"

#include "FQQuestTriggerVolume.h"
#include "FQMonsterKillQuest.h"
#include "FQMonsterKillQuestDataAsset.h"
#include "FQInteractionQuest.h"
#include "FQInteractionQuestDataAsset.h"

#include "EngineUtils.h"

AFQQuestManager::AFQQuestManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFQQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
    for (TActorIterator<AFQQuestTriggerVolume> It(GetWorld()); It; ++It)
    {
        AFQQuestTriggerVolume* Trigger = *It;
        if (Trigger)
        {
            Trigger->mQuestTriggerDelegate.BindUObject(this, &AFQQuestManager::OnTriggerCallbackFunction);
        }
    }
}

void AFQQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFQQuestManager::OnTriggerCallbackFunction(int32 QuestID, EQuestTriggerType QuestTrigeerType)
{
    UFQMonsterKillQuestDataAsset** MonsterKillQuestData = mMonsterKillQuestDataList.Find(QuestID);
    if (MonsterKillQuestData)
    {
        auto QuestObject = CreateDefaultSubobject<AFQMonsterKillQuest>(TEXT("MonsterKillQuest"));
        QuestObject->SetQuestID(QuestID);

        QuestObject->SetQuestDescription((*MonsterKillQuestData)->mDescription);
        QuestObject->SetMonsterType((*MonsterKillQuestData)->mMonsterType);
        QuestObject->SetClearMonsterKillNumber((*MonsterKillQuestData)->mMonsterKillNumber);
    }

    UFQInteractionQuestDataAsset** InteractionQuestData = mInteractionQuestDataList.Find(QuestID);
    if (InteractionQuestData)
    {
        auto QuestObject = CreateDefaultSubobject<AFQInteractionQuest>(TEXT("InteractionQuest"));
        QuestObject->SetQuestID(QuestID);

        QuestObject->SetQuestDescription((*InteractionQuestData)->mDescription);
        QuestObject->SetInteractionType((*InteractionQuestData)->mInteractionType);
    }
}