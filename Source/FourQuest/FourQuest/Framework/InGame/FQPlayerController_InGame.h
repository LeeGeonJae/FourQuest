// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FQGameCore\Controller\FQPlayerControllerInterface.h"
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
	virtual void SetSoulType(ESoulType InSoulType) override;
	virtual ESoulType GetSoulType() const override;

protected:
	// Parent Class Funtion
	virtual void BeginPlay() override;

private:
	// GameMode의 Horizontal Box에 추가할 Player HUD Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mPlayerHUDWidget;
};
