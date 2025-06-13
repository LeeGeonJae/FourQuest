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
	virtual void Update(float DeltaTime) override;

	FORCEINLINE void SetInteractionType(const EQuestInteractionType InteractionType) { mInteractionType = InteractionType; }
	FORCEINLINE EQuestInteractionType GetInteractionType() const { return mInteractionType; }

protected:
	virtual void BeginPlay() override;
	
private:
	EQuestInteractionType mInteractionType;
};
