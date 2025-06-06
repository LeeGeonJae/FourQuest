// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQMageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQMageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQMageDataAsset();

	// Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "기본 이동 속도"))
	float mDefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "체력"))
	float mHealth;

	// Projectile Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 쿨타임"))
	float mProjectileAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 1타 -> 2타 입력 대기 시간"))
	float mProjectileAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 2타 -> 1타 입력 대기 시간"))
	float mProjectileAttackWaitTime2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 관통 수"))
	int32 mProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 속도"))
	float mProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 지속 시간"))
	float mProjectileDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 밀어내는 힘"))
	float mProjectileStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,meta = (ToolTip = "[X] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mProjectileAttackableTypes;
};
