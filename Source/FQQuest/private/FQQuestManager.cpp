#include "FQQuestManager.h"

#include "FQGameCore\Common.h"
#include "FQQuestTriggerVolume.h"
#include "FQMonsterKillQuest.h"
#include "FQMonsterKillQuestDataAsset.h"
#include "FQNavigationQuest.h"
#include "FQNavigationQuestDataAsset.h"

#include "EngineUtils.h"

// Sets default values
AFQQuestManager::AFQQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
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

// Called every frame
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
    }

    UFQNavigationQuestDataAsset** NavigationQuestData = mNavigationQuestDataList.Find(QuestID);
    if (NavigationQuestData)
    {
        auto QuestObject = CreateDefaultSubobject<AFQNavigationQuest>(TEXT("NavigationQuest"));
        QuestObject->SetQuestID(QuestID);
    }
}