#include "FQQuestBase.h"

#include "FQQuestStartedState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestInProgressState.h"
#include "FQUI\Quest\FQQuestWidget.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

#include "FQMonsterKillQuest.h"
#include "FQInteractionQuest.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
	, mCurrentStateType(EQuestStateType::Started)
	, mQuestCurrentConditionNumber()
	, mQuestClearConditionNumber()
{
	ConstructorHelpers::FClassFinder<UUserWidget> QuestWidgetRef(TEXT("/Game/Blueprints/Quest/WBP_QuestWidget.WBP_QuestWidget_C"));
	if (QuestWidgetRef.Class)
	{
		mQuestWidget = CreateWidget<UFQQuestWidget>(GetWorld(), QuestWidgetRef.Class);
	}
}

void AFQQuestBase::BeginPlay()
{
	Super::BeginPlay();

	// UI
	mQuestWidget->SetQuestDescriptionText(mDescription);

	// State
	mCurrentState = NewObject<UFQQuestStartedState>(this);
	mCurrentState->SetOwnerQuestObject(this);
	mCurrentState->EnterState();
	UpdateQuestCondition(0);

	// 서브 퀘스트 생성
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		for (int32 SubQuestID : QuestSystem->GetQuestData(GetQuestID())->SubQuestList)
		{
			CreateSubQuest(SubQuestID);
		}
	}
}


void AFQQuestBase::UpdateQuest(float DeltaTime)
{
	mCurrentState->UpdateState(DeltaTime);

	if (!mQuestWidget)
	{
		return;
	}

	switch (mCurrentStateType)
	{
	case EQuestStateType::Started:
	{
		UFQQuestStartedState* MyState = Cast<UFQQuestStartedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
		}
	}
	break;
	case EQuestStateType::InPrograss:
		break;
	case EQuestStateType::Exit:
	{
		UFQQuestCompletedState* MyState = Cast<UFQQuestCompletedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
			mQuestWidget->UpdateQuestStateCheckBoxAnimation(MyState->GetDurationValue());
		}
	}
	break;
	}

	// 서브 퀘스트 업데이트
	for (auto mSubQuest : mSubQuestList)
	{
		mSubQuest.Value->UpdateQuest(DeltaTime);
	}
}

void AFQQuestBase::SetNewState(const EQuestStateType NewState)
{
	mCurrentStateType = NewState;
	switch (mCurrentStateType)
	{
	case EQuestStateType::InPrograss:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestInProgressState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : InPrograss"), __LINE__);
	}
	break;
	case EQuestStateType::Exit:
	{
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestCompletedState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : Exit"), __LINE__);
	}
	break;
	case EQuestStateType::End:
	{
		UE_LOG(LogTemp, Log, TEXT("[AFQQuestBase %d] 퀘스트 현재 상태 : End"), __LINE__);

		// 퀘스트 클리어 데이터 저장
		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem)
		{
			QuestSystem->GetQuestData(GetQuestID())->mbIsQuestClear = true;
		}
	}
	break;
	}

	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestState(mCurrentStateType);
	}
}

void AFQQuestBase::UpdateQuestCondition(int32 AddConditionNumber)
{
	if (mQuestCurrentConditionNumber == mQuestClearConditionNumber)
	{
		return;
	}

	mQuestCurrentConditionNumber += AddConditionNumber;
	if (mQuestCurrentConditionNumber >= mQuestClearConditionNumber)
	{
		SetNewState(EQuestStateType::Exit);
	}

	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestCondition(mQuestClearConditionNumber > mQuestCurrentConditionNumber ? mQuestCurrentConditionNumber : mQuestClearConditionNumber, mQuestClearConditionNumber);
	}
}

void AFQQuestBase::CreateSubQuest(int32 QuestID)
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
			mSubQuestList.Emplace(QuestData->QuestNumber, *MonsterKillQuest);

			UFQQuestWidget* MyQuestWidget = MonsterKillQuest->GetQuestWidget();
			if (MyQuestWidget)
			{
				mQuestWidget->AddSubQuestListWidget(MyQuestWidget);
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
			mSubQuestList.Emplace(QuestData->QuestNumber, *InteractionQuest);

			UFQQuestWidget* MyQuestWidget = InteractionQuest->GetQuestWidget();
			if (MyQuestWidget)
			{
				mQuestWidget->AddSubQuestListWidget(MyQuestWidget);
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