#include "FQQuestTriggerVolume.h"

#include "Components/BoxComponent.h"

#include "FQGameCore\Player\FQPlayerCharacterInterface.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

AFQQuestTriggerVolume::AFQQuestTriggerVolume()
	: mPlayerNumber()
{
	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(mTrigger);
	mTrigger->SetBoxExtent(FVector(100.f));
	mTrigger->SetCollisionProfileName(TEXT("Trigger"));
	mTrigger->SetGenerateOverlapEvents(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (mTrigger)
	{
		mTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFQQuestTriggerVolume::OnTriggerBeginOverlap);
		mTrigger->OnComponentEndOverlap.AddDynamic(this, &AFQQuestTriggerVolume::OnTriggerEndOverlap);
	}
}

void AFQQuestTriggerVolume::Tick(float DeltaTime)
{
	if (mPlayerNumber > 0)
	{
		mDurationTime += DeltaTime;
		if (mDurationTime < 1.f)
		{
			return;
		}
		mDurationTime = 0.f;
		UE_LOG(LogTemp, Log, TEXT("[AFQQuestTriggerVolume %d] 캡쳐 퀘스트 호출!!"), __LINE__);

		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Capture);
		}
	}
}

void AFQQuestTriggerVolume::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IFQPlayerCharacterInterface* CollisionPlayer = Cast<IFQPlayerCharacterInterface>(OtherActor);
	if (CollisionPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("[AFQQuestTriggerVolume %d] 플레이어 비긴 오버랩!!"), __LINE__);
		mPlayerNumber++;
	}
}

void AFQQuestTriggerVolume::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IFQPlayerCharacterInterface* CollisionPlayer = Cast<IFQPlayerCharacterInterface>(OtherActor);
	if (CollisionPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("[AFQQuestTriggerVolume %d] 플레이어 앤드 오버랩!!"), __LINE__);
		mPlayerNumber--;
	}
}
