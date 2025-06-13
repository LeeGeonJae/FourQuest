// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FQGameCore/Common.h"
#include "FQInterationQuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FQGAMECORE_API UFQInterationQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFQInterationQuestComponent();

	FQ_QuestDelegate::FQQuestInteractionDelegate mInteractionDelegate;

	// 퀘스트 호출
	void ExecuteQuest() { mInteractionDelegate.ExecuteIfBound(mInteractionType); }

	// 타입 지정
	FORCEINLINE void SetInteractionType(const EQuestInteractionType InteractionType) { mInteractionType = InteractionType; }
	FORCEINLINE EQuestInteractionType GetInteractionType() const { return mInteractionType; }
	
private:
	EQuestInteractionType mInteractionType;
};
