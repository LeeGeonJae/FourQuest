// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "Engine\TimerHandle.h"
#include "InputActionValue.h"

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
	virtual void SetupInputComponent() override;

protected:
	// Parent Class Funtion
	virtual void BeginPlay() override;

private:
	void CreatePlayerCharacterByClass(TSubclassOf<class AFQPlayerBase> CharacterClass, const FTransform& SpawnTransform);
	void CreateSoulCharacterByClass(TSubclassOf<class AFQSoulBase> CharacterClass, const FTransform& SpawnTransform);

	// UI Widget Input
	void HandlePickButton();
	void HandleCancelButton();
	void HandleMoveTriggered(const FInputActionValue& Value);
	void HandleMoveCompleted(const FInputActionValue& Value);
	void DoMove();

private:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mPickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mCancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	// GameMode의 Horizontal Box에 추가할 Player HUD Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mPlayerHUDWidget;

	UPROPERTY(EditAnywhere, Category = FQCharacter, Meta = (AllowPrivateAccess = "true"))
	TMap<EArmourType, TSubclassOf<class AFQPlayerBase>> mPlayerArmourCharacterClasses;

	UPROPERTY(EditAnywhere, Category = FQCharacter, Meta = (AllowPrivateAccess = "true"))
	TMap<ESoulType, TSubclassOf<class AFQSoulBase>> mPlayerSoulCharacterClasses;

private:
	uint8 mbIsMoveKeyHeld : 1;
	FVector2D mMoveDirection;
	FTimerHandle mRepeatMoveTimerHandle;
};

