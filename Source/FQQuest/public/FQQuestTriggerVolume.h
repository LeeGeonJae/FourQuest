#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Common.h"
#include "FQQuestTriggerVolume.generated.h"

UCLASS()
class FQQUEST_API AFQQuestTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestTriggerVolume();

public:
	// 퀘스트 매니저 델리게이트
	FQ_QuestDelegate::FQQuestTriggerDelegate mQuestTriggerDelegate;

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	// 퀘스트 트리거 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQQuestTriggerType)
	TMap<int32, EQuestTriggerType> mQuestTriggerTypeList;

	// 충돌 박스
	UPROPERTY(EditAnywhere, Category = FQBox, Category = FQBox)
	TObjectPtr<class UBoxComponent> mTrigger;

private:
};
