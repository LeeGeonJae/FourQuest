// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestBase.h"
#include "FQNavigationQuest.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API AFQNavigationQuest : public AFQQuestBase
{
	GENERATED_BODY()
	
public:
	AFQNavigationQuest();

	virtual void Execute() override;
	virtual void Update(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

};
