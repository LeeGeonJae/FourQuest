// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
#include "FQUI\UI\FQSoulSelectScreenWidget.h"
#include "FQGameMode_SoulSelect.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_SoulSelect : public AGameModeBase, public IFQGameModeInterface
{
	GENERATED_BODY()
	
public:
	AFQGameMode_SoulSelect();

	// Interface virtual Funtion
	virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) override;
	virtual void CancelInteraction(int32 ControllerId) override;
	virtual void SelectInteraction(int32 ControllerId) override;

protected:
	// Parent virtual Function
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void CreatePlayer();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQTitle, Meta = (AllowPrivateAccess = "true"))
	FName mTargetLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFQSoulSelectScreenWidget> mSoulSelectWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFQSoulSelectScreenWidget> mSoulSelectWidgetHandle;

	TMap<uint8, EPlayerStateType> mPlayerStates;
	TMap<uint8, ESoulType> mPlayerSoulTypes;
};
