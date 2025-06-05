// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
#include "FQGameMode_Title.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_Title : public AGameModeBase, public IFQGameModeInterface
{
	GENERATED_BODY()
	
public:
	AFQGameMode_Title();

	// Interface Funtion
	virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) override;
	virtual void CancelInteraction(int32 ControllerId) override;
	virtual void SelectInteraction(int32 ControllerId) override;

protected:
	// Parent Function
	virtual void BeginPlay() override;

private:
	void StartGame();
	void ExitGame();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQTitle, Meta = (AllowPrivateAccess = "true"))
	FName mTargetLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UFQTitleScreenWidget> mTitleWidgetClass;
	UPROPERTY()
	TObjectPtr<class UFQTitleScreenWidget> mTitleWidgetHandle;
};
