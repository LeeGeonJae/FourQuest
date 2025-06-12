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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
