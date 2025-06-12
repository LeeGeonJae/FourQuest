// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQQuestManager.generated.h"

UCLASS()
class FQQUEST_API AFQQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = FQQuestList)
	TArray<TObjectPtr<class UFQMonsterKillQuestDataAsset>> mMonsterKillQuestList;
	UPROPERTY(EditAnywhere, Category = FQQuestList)
	TArray<TObjectPtr<class UFQNavigationQuestDataAsset>> mNavigationQuestList;
	
	UPROPERTY()
	TArray<TObjectPtr<class AFQQuestBase>> mQuestList;
};
