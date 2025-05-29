// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerAnimInstance.h"
#include "FQKnightAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQKnightAnimInstance : public UFQPlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	UFQKnightAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float mShieldMoveAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 mbIsHoldShield : 1;
};
