// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FQGameCore\Player\FQPlayerControllerInterface.h"
#include "FQGameCore\Common.h"
#include "FQPlayerController_InGame.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQPlayerController_InGame : public APlayerController, public IFQPlayerControllerInterface
{
	GENERATED_BODY()	
	
public:
	AFQPlayerController_InGame();

	// Interface Class Funtion
	virtual void ChangeToArmour(EArmourType InArmourType) override;
	virtual void ChangeToSoul() override;

protected:
	// Parent Class Funtion
	virtual void BeginPlay() override;

private:
	void CreatePlayerCharacterByClass(TSubclassOf<class AFQPlayerBase> CharacterClass, const FTransform& SpawnTransform);
	void CreateSoulCharacterByClass(TSubclassOf<class AFQSoulBase> CharacterClass, const FTransform& SpawnTransform);

private:
	// GameMode의 Horizontal Box에 추가할 Player HUD Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mPlayerHUDWidget;

	UPROPERTY(EditAnywhere, Category = Character)
	TMap<EArmourType, TSubclassOf<class AFQPlayerBase>> mPlayerArmourCharacterClasses;

	UPROPERTY(EditAnywhere, Category = Character)
	TMap<ESoulType, TSubclassOf<class AFQSoulBase>> mPlayerSoulCharacterClasses;
};

