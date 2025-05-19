// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerAnimInstance.h"
#include "FQPlayer/Public/FQPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UFQPlayerAnimInstance::UFQPlayerAnimInstance()
{
	mMovingThreshould = 3.0f;
}

void UFQPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	mOwner = Cast<AFQPlayerBase>(GetOwningActor());
	if (mOwner)
	{
		mMovement = Cast<UCharacterMovementComponent>(mOwner->GetCharacterMovement());
	}
}

void UFQPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!mMovement)
	{
		return;
	}

	mVelocity = mMovement->Velocity;
	mGroundSpeed = mVelocity.Size2D();
	mNormalizedSpeed = mGroundSpeed / mMovement->GetMaxSpeed();
	mbIsIdle = mGroundSpeed < mMovingThreshould;
}

void UFQPlayerAnimInstance::SetHitReacting(bool HitReacting)
{
	if (!mOwner)
	{
		return;
	}

	mbHitReacting = HitReacting;
	mOwner->SetHitReacting(mbHitReacting);	
}

void UFQPlayerAnimInstance::ProcessNextSection(const FName& SectionName)
{
	if (!mOwner)
	{
		return;
	}

	mOwner->ProcessNextSection(SectionName);
}
