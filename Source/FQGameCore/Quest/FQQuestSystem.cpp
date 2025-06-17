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
	auto MyQuestData = mQuestTableData.Find(QuestNumber);
	if (MyQuestData)
	{
		return MyQuestData;
	}
	return nullptr;
}