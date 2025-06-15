#include "FQQuestTriggerVolume.h"

#include "Components/BoxComponent.h"

#include "FQGameCore\Player\FQPlayerCharacterInterface.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

AFQQuestTriggerVolume::AFQQuestTriggerVolume()
{
	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(mTrigger);
	mTrigger->SetBoxExtent(FVector(100.f));
	mTrigger->SetCollisionProfileName(TEXT("Trigger"));
	mTrigger->SetGenerateOverlapEvents(true);
}

void AFQQuestTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (mTrigger)
	{
		mTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFQQuestTriggerVolume::OnTriggerBeginOverlap);
	}
}

void AFQQuestTriggerVolume::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IFQPlayerCharacterInterface* CollisionPlayer = Cast<IFQPlayerCharacterInterface>(OtherActor);
	if (CollisionPlayer)
	{
		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (!QuestSystem)
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQQuestTriggerVolume %d] UFQQuestSystem가 유효하지 않습니다!!"), __LINE__);
			return;
		}

		for (auto Quest : mQuestTriggerTypeList)
		{
			QuestSystem->mQuestTriggerDelegate.ExecuteIfBound(Quest.Key, Quest.Value);
		}
	}
}
