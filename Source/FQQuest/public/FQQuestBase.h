// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Common.h"
#include "FQQuestBase.generated.h"

UCLASS()
class FQQUEST_API AFQQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestBase();

	// 가상 함수
	virtual void Execute() PURE_VIRTUAL(AFQQuestBase::Execute, );
	virtual void UpdateQuest(float DeltaTime);

	// 일반 함수
	void SetNewState(const EQuestStateType NewState);

	// 겟셋 함수
	FORCEINLINE EQuestStateType GetCurrentState() const { return mCurrentStateType; }
	FORCEINLINE void SetQuestID(const int32 QuestID) { mQuestID = QuestID; }
	FORCEINLINE int32 GetQuestID() const { return mQuestID; }
	FORCEINLINE void SetQuestDescription(const FString Description) { mDescription = Description; }
	FORCEINLINE FString GetQuestDescription() const { return mDescription; }

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	int32 mQuestID;
	FString mDescription;

	EQuestStateType mCurrentStateType;

	UPROPERTY()
	TObjectPtr<class UFQQuestStateBase> mCurrentState;

	UPROPERTY()
	TObjectPtr<class UFQQuestWidget> mQuestWidget;
};
