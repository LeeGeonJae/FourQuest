// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQQuestBase.generated.h"

UCLASS()
class FQQUEST_API AFQQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestBase();

	virtual void Execute() PURE_VIRTUAL(AFQQuestBase::Execute, );
	virtual void Update(float DeltaTime);

	void SetNewState(class UFQQuestStateBase* NewState);
	class UFQQuestStateBase* GetCurrentState() const { return mCurrentState; }
	FORCEINLINE void SetQuestID(const int32 QuestID) { mQuestID = QuestID; }
	FORCEINLINE int32 GetQuestID() const { return mQuestID; }
	FORCEINLINE void SetQuestDescription(const FString Description) { mDescription = Description; }
	FORCEINLINE FString GetQuestDescription() const { return mDescription; }

protected:
	virtual void BeginPlay() override;

private:
	int32 mQuestID;
	FString mDescription;

	UPROPERTY()
	TObjectPtr<class UFQQuestStateBase> mCurrentState;

	UPROPERTY()
	TObjectPtr<class UFQUserWidget> mQuestWidget;
};
