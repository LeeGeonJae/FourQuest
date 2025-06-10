// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQMonsterBase.h"
#include "FQBossMonster.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API AFQBossMonster : public AFQMonsterBase
{
	GENERATED_BODY()

public:
	AFQBossMonster();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float mGrawlCooltime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float mRushCooltime = 10.0f;

private:
	uint8 mbCanGrawl : 1;
	uint8 mbCanRush : 1;

	FTimerHandle mGrawlCoolTimer;
	FTimerHandle mRushCoolTimer;
};
