#include "FQQuestManager.h"

#include "FQQuestTriggerVolume.h"
#include "FQMonsterKillQuest.h"
#include "FQInteractionQuest.h"
#include "FQGameCore\Quest\FQQuestSystem.h"
#include "FQUI/Quest/FQQuestListUI.h"
#include "FQUI/Quest/FQQuestWidget.h"

#include "EngineUtils.h"

AFQQuestManager::AFQQuestManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestManager::BeginPlay()
{
	Super::BeginPlay();

    // UI 생성
    if (mQuestListUIClass)
    {
        mQuestListUI = CreateWidget<UFQQuestListUI>(GetWorld(), mQuestListUIClass);
        if (mQuestListUI)
        {
            mQuestListUI->AddToViewport(); // 화면에 위젯 추가
            UE_LOG(LogTemp, Log, TEXT("[AFQQuestManager %d] Create HUD Widget And AddViewport"), __LINE__);
        }
    }
	
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

    TArray<int32> QuestsToRemove;
    for (auto& MyQuest : mQuestList)
    {
        if (!MyQuest.Value)
        {
            QuestsToRemove.Add(MyQuest.Key);
            return;
        }

        // 퀘스트 업데이트
        MyQuest.Value->UpdateQuest(DeltaTime);

        // 퀘스트 삭제
        EQuestStateType MyStateType = MyQuest.Value->GetCurrentState();
        if (MyStateType == EQuestStateType::End)
        {
            QuestsToRemove.Add(MyQuest.Key);
        }
        else if (MyStateType == EQuestStateType::Exit)
        {
            // 퀘스트 시스템
            UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
            if (QuestSystem)
            {
                if (QuestSystem->GetQuestData(MyQuest.Key)->mbIsQuestClear)
                {
                    continue;
                }

                // 다음 퀘스트 생성
                for (const int32& NextQuestID : QuestSystem->GetQuestData(MyQuest.Key)->NextQuestList)
                {
                    CreateQuest(NextQuestID);
                }
            }
        }
    }

    // 퀘스트 삭제
    for (int32 Key : QuestsToRemove)
    {
        ClearQuest(Key);
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
    // 퀘스트 시스템 탐색
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        FFQQuestTable* QuestData = QuestSystem->GetQuestData(QuestID);

        // 퀘스트 생성
        switch (QuestData->QuestType)
        {
        case EQuestType::MonsterKill:
        {
            AFQMonsterKillQuest* MonsterKillQuest = GetWorld()->SpawnActorDeferred<AFQMonsterKillQuest>(AFQMonsterKillQuest::StaticClass(), FTransform());
            MonsterKillQuest->SetQuestID(QuestData->QuestNumber);
            MonsterKillQuest->SetQuestClearConditionNumber(QuestData->QuestClearConditionsNumber);
            MonsterKillQuest->SetQuestDescription(QuestData->QuestDescription);
            MonsterKillQuest->SetQuestMonsterType(QuestData->QuestMonsterType);
            MonsterKillQuest->FinishSpawning(FTransform());
            mQuestList.Emplace(QuestData->QuestNumber, *MonsterKillQuest);

            UFQQuestWidget* MyQuestWidget = MonsterKillQuest->GetQuestWidget();
            if (MyQuestWidget)
            {
                mQuestListUI->AddQuestListWidget(MyQuestWidget);
            }
        }
        break;
        case EQuestType::Interaction:
        {
            AFQInteractionQuest* InteractionQuest = GetWorld()->SpawnActorDeferred<AFQInteractionQuest>(AFQInteractionQuest::StaticClass(), FTransform());
            InteractionQuest->SetQuestID(QuestData->QuestNumber);
            InteractionQuest->SetQuestClearConditionNumber(QuestData->QuestClearConditionsNumber);
            InteractionQuest->SetQuestInteractionType(QuestData->QuestInteractionType);
            InteractionQuest->SetQuestDescription(QuestData->QuestDescription);
            InteractionQuest->FinishSpawning(FTransform());
            mQuestList.Emplace(QuestData->QuestNumber, *InteractionQuest);

            UFQQuestWidget* MyQuestWidget = InteractionQuest->GetQuestWidget();
            if (MyQuestWidget)
            {
                mQuestListUI->AddQuestListWidget(MyQuestWidget);
            }
        }
        break;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQQuestManager %d] QuestSystem가 유효하지 않습니다!!"), __LINE__);
    }
}

void AFQQuestManager::ClearQuest(int32 QuestID)
{
    // UI 삭제
    UFQQuestWidget* MyQuestWidget = mQuestList[QuestID]->GetQuestWidget();
    if (MyQuestWidget)
    {
        mQuestListUI->RemoveQuestWidget(MyQuestWidget);
    }

    // 퀘스트 삭제
    mQuestList[QuestID]->Destroy();
    mQuestList.Remove(QuestID);
}
