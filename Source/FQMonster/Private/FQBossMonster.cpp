// Fill out your copyright notice in the Description page of Project Settings.


#include "FQBossMonster.h"
#include "FQMonsterManager.h"
#include "FQMonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AFQBossMonster::AFQBossMonster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFQBossMonster::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFQBossMonster::OnChargeHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFQBossMonster::OnAttackOverlap);

	mOriginalBrakingFrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
	mOriginalGravityScale = GetCharacterMovement()->GravityScale;

	if (mBossMonsterDataAsset)
	{
		mCurrentHP = mBossMonsterDataAsset->mMaxHP;
		mSPP = mBossMonsterDataAsset->mInitSPP;
		GetCharacterMovement()->MaxWalkSpeed = mBossMonsterDataAsset->mChaseSpeed;

	}

	mDownGauge = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(mDownDecreaseTimer, [this]() { mDownGauge -= 0.5f; }, 1.0f, true);
}

void AFQBossMonster::Attack()
{
	if (mAttackMontage)
	{
		const TArray<FName> Sections = { TEXT("Attack1"), TEXT("Attack2") };
		int32 Index = FMath::RandRange(0, Sections.Num() - 1);
		PlayAnimMontage(mAttackMontage, 1.0f, Sections[Index]);
	}
}

void AFQBossMonster::StrikeAttack()
{
	if (mAttackMontage)
	{
		FName SectionName = TEXT("Attack3");
		PlayAnimMontage(mAttackMontage, 1.0f, SectionName);
	}
}

void AFQBossMonster::Growl()
{
	if (Manager)
	{
		Manager->ChangeTargetActor(mGroupID, mTargetActor);
	}
}

void AFQBossMonster::PrepareRush()
{
	ChangeState(EMonsterState::Rush);
	if (mTargetActor)
	{
		mRushDirection = (mTargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		FRotator Rot = mRushDirection.Rotation();
		FRotator NewRot = FRotator(0.f, Rot.Yaw, 0.f);
		SetActorRotation(NewRot);
	}
}

void AFQBossMonster::Rush()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->MaxFlySpeed = 1500.f;
	
	
	GetCharacterMovement()->Velocity = mRushDirection * mBossMonsterDataAsset->mRushSpeed;

	//bIsCharging = true;
}

void AFQBossMonster::EndRush()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->BrakingFrictionFactor = mOriginalBrakingFrictionFactor;
	GetCharacterMovement()->GravityScale = mOriginalGravityScale;
	ChangeState(EMonsterState::Chase);
}

void AFQBossMonster::SelectAttack()
{
	int32 RandInt = FMath::RandRange(0, 1);
	if (RandInt)
	{
		StrikeAttack();
	}
	else
	{
		Attack();
	}
}

void AFQBossMonster::SelectSkill()
{
	int32 RandInt = FMath::RandRange(1, 10);
	if (RandInt <= 4)
	{
		PrepareRush();
	}
	else if (RandInt >= 8)
	{
		Growl();
	}
	else
	{
		SelectAttack();
	}
}

void AFQBossMonster::Choose()
{
	int32 RandInt = FMath::RandRange(1, 100);
	if (RandInt < mSPP)
	{
		SelectSkill();
		mSPP = mBossMonsterDataAsset->mInitSPP;
	}
	else
	{
		SelectAttack();
		mSPP += mBossMonsterDataAsset->mAttackSPP;
	}
}

void AFQBossMonster::Hit(FVector AttackPos)
{
	if (mHitMontage)
	{
		FVector ForwardVector = GetActorForwardVector();
		float DotResult = FVector::DotProduct(ForwardVector, AttackPos);
		if (DotResult > 0.5)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitFront"));
		}
		else if (DotResult < -0.5)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitBack"));
		}
		else
		{
			if (FVector::CrossProduct(ForwardVector, AttackPos).Z > 0)
			{
				PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitLeft"));
			}
			else
			{
				PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitRight"));
			}
		}
	}
}

float AFQBossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[BossMonster] TakeDamage"));

		AIC->ChangeState(EMonsterState::Hit);
		Hit((DamageCauser->GetActorLocation()-GetActorLocation()).GetSafeNormal());
		if(mMonsterState!=EMonsterState::Down)
		{
			mCurrentHP = mCurrentHP - DamageAmount;
		}
		else
		{
			mCurrentHP = mCurrentHP - (DamageAmount*mBossMonsterDataAsset->mDownHitDamage);
		}
		mDownGauge += 5.0f;
		if (mDownGauge >= 100)
		{
			AIC->ChangeState(EMonsterState::Down);
		}
	}
	return DamageAmount;
}

void AFQBossMonster::OnChargeHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!OtherActor->ActorHasTag("Floor"))
	{
		EndRush();
	}
}

void AFQBossMonster::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(mMonsterState==EMonsterState::Rush)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			FVector ToTarget = (mTargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector KnockbackDir;
			float CrossZ = FVector::CrossProduct(GetVelocity().GetSafeNormal(), ToTarget).Z;
			if (CrossZ > 0)
			{
				// 오른쪽 넉백
				KnockbackDir = FVector::CrossProduct(FVector::UpVector, GetVelocity().GetSafeNormal()).GetSafeNormal();
				UE_LOG(LogTemp, Log, TEXT("Right knockback"));
			}
			else
			{
				// 왼쪽 넉백
				KnockbackDir = FVector::CrossProduct(GetVelocity().GetSafeNormal(), FVector::UpVector).GetSafeNormal();
				UE_LOG(LogTemp, Log, TEXT("Left knockback"));
			}

			float KnockbackForce = 3000.f;
			Character->LaunchCharacter(KnockbackDir * KnockbackForce, true, true);
			UGameplayStatics::ApplyDamage(Character, mBossMonsterDataAsset->mRushDamage, GetController(), this, UDamageType::StaticClass());
		}
	}
}
