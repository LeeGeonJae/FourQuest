// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQQuestStartedState.h"
#include "FQQuestInProgressState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestBase.generated.h"

UCLASS()
class FQQUEST_API AFQQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestBase();

	virtual void Execute() abstract;
	virtual void Update(float DeltaTime);

	void SetNewState(UFQQuestStateBase* NewState);
	UFQQuestStateBase* GetCurrentState() { return mCurrentState; }
	FORCEINLINE void SetQuestID(int32 QuestID) { mQuestID = QuestID; }
	FORCEINLINE int32 GetQuestID() { return mQuestID; }

protected:
	virtual void BeginPlay() override;

protected:


private:
	int32 mQuestID;
	TObjectPtr<UFQQuestStateBase> mCurrentState;
};
