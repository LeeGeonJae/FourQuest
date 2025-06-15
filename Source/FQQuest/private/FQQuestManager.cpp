#include "FQQuestManager.h"

#include "FQQuestTriggerVolume.h"
#include "FQMonsterKillQuest.h"
#include "FQInteractionQuest.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

#include "EngineUtils.h"

AFQQuestManager::AFQQuestManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
    // 콜백 함수 등록
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        QuestSystem->mQuestTriggerDelegate.BindUObject(this, &AFQQuestManager::OnTriggerCallbackFunction);
    }
    
    // 첫 퀘스트 시작 번호 생성
    CreateQuest(1);
}

void AFQQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    for (auto& MyQuest : mQuestList)
    {
        // 퀘스트 업데이트
        MyQuest.Value->UpdateQuest(DeltaTime);
    }
}

void AFQQuestManager::OnTriggerCallbackFunction(int32 QuestID, EQuestTriggerType QuestTrigeerType)
{
	switch (QuestTrigeerType)
	{
	case EQuestTriggerType::QuestStarts:
	{
        // 퀘스트 생성
		CreateQuest(QuestID);
	}
	break;
    case EQuestTriggerType::QuestClear:
	{
        auto MyQuest = mQuestList.Find(QuestID);
        if (MyQuest)
        {
            (*MyQuest)->SetNewState(EQuestStateType::Exit);
        }
	}
	break;
	}
}

void AFQQuestManager::CreateQuest(int32 QuestID)
{
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        FFQQuestTable QuestData = QuestSystem->GetQuestData(QuestID);
        switch (QuestData.QuestType)
        {
        case EQuestType::MonsterKill:
        {
            AFQMonsterKillQuest* MonsterKillQuest = CreateDefaultSubobject<AFQMonsterKillQuest>(TEXT("MonsterKillQuest"));
            MonsterKillQuest->SetQuestID(QuestData.QuestNumber);
            MonsterKillQuest->SetQuestClearMonsterKillNumber(QuestData.QuestClearConditionsNumber);
            MonsterKillQuest->SetQuestDescription(QuestData.QuestDescription);
            MonsterKillQuest->SetQuestMonsterType(QuestData.QuestMonsterType);
            mQuestList.Emplace(QuestData.QuestNumber, *MonsterKillQuest);
        }
        break;
        case EQuestType::Interaction:
        {
            AFQInteractionQuest* InteractionQuest = CreateDefaultSubobject<AFQInteractionQuest>(TEXT("InteractionQuest"));
            InteractionQuest->SetQuestID(QuestData.QuestNumber);
            InteractionQuest->SetQuestInteractionType(QuestData.QuestInteractionType);
            InteractionQuest->SetQuestDescription(QuestData.QuestDescription);
            mQuestList.Emplace(QuestData.QuestNumber, *InteractionQuest);
        }
        break;
        }
    }
}
