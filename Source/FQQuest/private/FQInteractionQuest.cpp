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
	// 퀘스트 시스템
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(GetQuestID());

		// 서브 퀘스트 클리어 여부 확인
		TArray<int32> SubQuestIDData = MyQuestData->SubQuestList;
		for (auto SubQuestID : SubQuestIDData)
		{
			if (!QuestSystem->GetQuestData(SubQuestID)->mbIsQuestClear)
			{
				return;
			}
		}
	}

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