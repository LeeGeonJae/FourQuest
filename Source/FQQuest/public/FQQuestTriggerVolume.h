// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQQuestTriggerVolume.generated.h"

UCLASS()
class FQQUEST_API AFQQuestTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestTriggerVolume();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
