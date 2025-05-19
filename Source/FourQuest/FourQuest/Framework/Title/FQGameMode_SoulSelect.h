// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameMode_SoulSelect.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_SoulSelect : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFQGameMode_SoulSelect();

protected:
	virtual void BeginPlay() override;
};
