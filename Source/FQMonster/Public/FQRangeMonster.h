// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQMonsterBase.h"
#include "FQRangeMonster.generated.h"

class AFQRangeMonsterProjectile;

/**
 * 
 */
UCLASS()
class FQMONSTER_API AFQRangeMonster : public AFQMonsterBase
{
	GENERATED_BODY()
	
public:
	virtual void Attack(AActor* Target)override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AFQRangeMonsterProjectile> mProjectile;
};
