#include "FQMonsterKillQuest.h"

#include "FQGameCore\Quest/FQQuestSystem.h"

AFQMonsterKillQuest::AFQMonsterKillQuest()
    : mMonsterType(EQuestMonsterType::None)
{
}

void AFQMonsterKillQuest::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 등록
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		QuestSystem->mMonsterQuestDelegate.AddUObject(this, &AFQMonsterKillQuest::TryUpdateQuestState);
	}
}

void AFQMonsterKillQuest::TryUpdateQuestState(EQuestMonsterType MonsterType)
{
	// 퀘스트 시스템
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		// 현재 클리어 상태이면 종료
		if (QuestSystem->GetQuestData(GetQuestID())->mbIsQuestClear)
		{
			return;
		}

		// 서브 퀘스트 클리어 여부 확인
		FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(GetQuestID());
		TArray<int32> SubQuestIDData = MyQuestData->SubQuestList;
		for (auto SubQuestID : SubQuestIDData)
		{
			if (!QuestSystem->GetQuestData(SubQuestID)->mbIsQuestClear)
			{
				return;
			}
		}
	}

	// 퀘스트 조건 갱신
    if (mMonsterType == MonsterType)
    {
		UpdateQuestCondition(1);
    }
}

void AFQMonsterKillQuest::Execute()
{
}

void AFQMonsterKillQuest::UpdateQuest(float DeltaTime)
{
	Super::UpdateQuest(DeltaTime);


}
