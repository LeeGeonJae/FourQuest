// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SwordAttack.h"
#include "FQPlayer/Public/FQKnightPlayer.h"

void UAnimNotifyState_SwordAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AFQKnightPlayer* Player = Cast<AFQKnightPlayer>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->EnableSwordAttackVolume();
}

void UAnimNotifyState_SwordAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AFQKnightPlayer* Player = Cast<AFQKnightPlayer>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->DisableSwordAttackVolume();
}
