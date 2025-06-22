// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMeleeMonster.h"
#include "FQMonsterAIController.h"

#include "Components/CapsuleComponent.h"

void AFQMeleeMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AFQMeleeMonster::Hit()
{
	if (mHitMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Velocity Scalar : %f"), GetVelocity().Size2D())
		if (GetVelocity().Size2D() > 0)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("Push"));
			return;
		}
		const TArray<FName> Sections = { TEXT("Hit1"), TEXT("Hit2") };
		int32 Index = FMath::RandRange(0, Sections.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Hit Animation"))
		PlayAnimMontage(mHitMontage, 1.0f, Sections[Index]);
	}
}

float AFQMeleeMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[MeleeMonster] TakeDamage"));

		Hit();
		AIC->ChangeTargetActor(DamageCauser);
		mCurrentHP = mCurrentHP - DamageAmount;
		mCurrentHPPercent = mCurrentHP / mMonsterDataAsset->mMaxHP;
		if (mCurrentHP <= 0)
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AIC->ChangeState(EMonsterState::Death);
		}
	}

	return DamageAmount;
}

void AFQMeleeMonster::TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength)
{
	Super::TakePushByPlayer(Target, Direction, Strength);
	Hit();
}