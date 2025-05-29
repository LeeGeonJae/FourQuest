// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/AnimNotify_ProcessNextSection.h"
#include "FQPlayer/Public/FQKnightPlayer.h"

void UAnimNotify_ProcessNextSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	AFQKnightPlayer* Player = Cast<AFQKnightPlayer>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->ProcessNextSection();
}
