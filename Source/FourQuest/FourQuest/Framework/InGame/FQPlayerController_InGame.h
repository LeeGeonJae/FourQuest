// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FQGameCore\Controller\FQPlayerControllerInterface.h"
#include "FQGameCore\Soul\CommonSoul.h"
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

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

	// Interface
	virtual void SetSoulType(ESoulType InSoulType) override;
	virtual ESoulType GetSoulType() override;

private:
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mPlayerHUDWidget;

	ESoulType mSoulType;
};
