#include "FQQuestPoint.h"

#include "FQUI/Quest/FQQuestPointWidget.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

AFQQuestPoint::AFQQuestPoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFQQuestPoint::BeginPlay()
{
	Super::BeginPlay();
	
    // UI 생성
    if (mQuestPointWidgetClass)
    {
        mQuestPointWidget = CreateWidget<UFQQuestPointWidget>(GetWorld(), mQuestPointWidgetClass);
        if (mQuestPointWidget)
        {
            mQuestPointWidget->AddToViewport(); // 화면에 위젯 추가
            UE_LOG(LogTemp, Log, TEXT("[AFQQuestManager %d] Create HUD Widget And AddViewport"), __LINE__);
        }
    }

    // 콜백 함수 등록
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        QuestSystem->mQuestActiveDelegate.AddUObject(this, &AFQQuestPoint::UpdateQuestActive);

        // UI
        mQuestPointWidget->UpdateQuestActive(QuestSystem->GetQuestData(mQuestID)->mbIsActive);
    }

    mQuestPointWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AFQQuestPoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (mQuestPointWidget->GetVisibility() == ESlateVisibility::Visible)
    {

    }
}

void AFQQuestPoint::SetVisibleWidget(bool IsVisible)
{
    if (IsVisible)
    {
        mQuestPointWidget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        mQuestPointWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AFQQuestPoint::UpdateQuestActive(int32 QuestID, bool bIsQuestActive)
{
    if (mQuestID == QuestID)
    {
        mQuestPointWidget->UpdateQuestActive(bIsQuestActive);
    }
}

