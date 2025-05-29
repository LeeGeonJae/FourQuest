// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"
#include "FQMonsterBase.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFQMonsterBase* Monster = Cast<AFQMonsterBase>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->ApplyDamageToTarget();
	}
}
