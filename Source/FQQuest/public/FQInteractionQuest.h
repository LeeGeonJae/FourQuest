// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestBase.h"
#include "FQGameCore\Common.h"
#include "FQInteractionQuest.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API AFQInteractionQuest : public AFQQuestBase
{
	GENERATED_BODY()
	
public:
	AFQInteractionQuest();
	
	virtual void Execute() override;
	virtual void UpdateQuest(float DeltaTime) override;

	FORCEINLINE void SetQuestInteractionType(const EQuestInteractionType InteractionType) { mInteractionType = InteractionType; }
	FORCEINLINE EQuestInteractionType GetQuestInteractionType() const { return mInteractionType; }

protected:
	virtual void BeginPlay() override;

private:
	// 콜백 함수
	void TryUpdateQuestState(EQuestInteractionType InteractionType);

private:
	EQuestInteractionType mInteractionType;
};
