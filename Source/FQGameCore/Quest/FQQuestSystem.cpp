#include "FQQuestSystem.h"

UFQQuestSystem::UFQQuestSystem()
{
}

void UFQQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// DataTable 경로 설정
	const FString TablePath = TEXT("/Game/Data/DT_QuestTable.DT_QuestTable");

	// DataTable 로드
	UDataTable* LoadedTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TablePath));
	if (!LoadedTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestSystem %d] Failed to load QuestTable from path: %s"), __LINE__, *TablePath);
		return;
	}

	// 모든 Row 로드
	const TArray<FName> RowNames = LoadedTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		static const FString ContextString(TEXT("QuestTable Load Context"));
		if (FFQQuestTable* RowData = LoadedTable->FindRow<FFQQuestTable>(RowName, ContextString))
		{
			mQuestTableData.Add(RowData->QuestNumber, *RowData); // 복사
		}
	}
}

FFQQuestTable* UFQQuestSystem::GetQuestData(int32 QuestNumber)
{
	// 퀘스트 업데이트 체크 및 반환
	auto MyQuestData = mQuestTableData.Find(QuestNumber);
	if (MyQuestData != nullptr)
	{
		MyQuestData->mbIsActive = true;
		for (int32 SubQusetID : MyQuestData->SubQuestList)
		{
			FFQQuestTable* SubQuestData = mQuestTableData.Find(SubQusetID);
			if (SubQuestData && !SubQuestData->mbIsQuestClear)
			{
				MyQuestData->mbIsActive = false;
				break;
			}
		}
		
		// 퀘스트 활성화 델리게이트 호출
		mQuestActiveDelegate.Broadcast(QuestNumber, MyQuestData->mbIsActive);
		return MyQuestData;
	}
	else
	{
		return nullptr;
	}
}