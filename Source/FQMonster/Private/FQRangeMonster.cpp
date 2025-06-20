// Fill out your copyright notice in the Description page of Project Settings.


#include "FQRangeMonster.h"
#include "FQRangeMonsterProjectile.h"
#include "FQMonsterAIController.h"


void AFQRangeMonster::ProjectileAttack()
{
    if (!mTargetActor || !mProjectile) return;

    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50.f); // 약간 위로
    FRotator SpawnRotation = (mTargetActor->GetActorLocation() - SpawnLocation).Rotation();

    FActorSpawnParameters Params;
    Params.Instigator = this;
    Params.Owner = this;

    AFQRangeMonsterProjectile* Projectile = GetWorld()->SpawnActor<AFQRangeMonsterProjectile>(mProjectile, SpawnLocation, SpawnRotation, Params);
    if (Projectile)
    {
        if (mMonsterDataAsset)
            Projectile->SetDamage(mMonsterDataAsset->mAttackPower); // 예시 데미지
    }
}

float AFQRangeMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
    {
        UE_LOG(LogTemp, Warning, TEXT("[RnageMonster] TakeDamage"));

        AIC->ChangeTargetActor(DamageCauser);
        mCurrentHP = mCurrentHP - DamageAmount;
    }

    return DamageAmount;
}
