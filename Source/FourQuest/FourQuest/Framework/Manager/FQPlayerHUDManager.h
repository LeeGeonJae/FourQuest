// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Actor.h"
#include "FQPlayerHUDManager.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQPlayerHUDManager : public AActor
{
	GENERATED_BODY()
	
public:
	AFQPlayerHUDManager();

	void AddPlayerController(APlayerController* NewPlayer);

protected:
	virtual void BeginPlay() override;

private:
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TMap<uint8, TObjectPtr<class UUserWidget>> mPlayerHUDs;
};
