#include "FQQuestTriggerVolume.h"

#include "Components/BoxComponent.h"

#include "FQGameCore\Player\FQPlayerCharacterInterface.h"

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
		for (auto Quest : mQuestTriggerTypeList)
		{
			mQuestTriggerDelegate.ExecuteIfBound(Quest.Key, Quest.Value);
		}
	}
}
