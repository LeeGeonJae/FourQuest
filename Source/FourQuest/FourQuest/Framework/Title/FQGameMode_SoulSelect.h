// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
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

	// Interface Funtion
	virtual void MoveButton(const FInputActionValue& Value) override;
	virtual void CancelInteraction() override;
	virtual void SelectInteraction() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UFQSoulSelectWidget> mSoulSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<class UFQSoulSelectWidget> mSoulSelectWidgetHandle;
};
