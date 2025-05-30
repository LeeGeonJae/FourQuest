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

	// Common Funtion
	void AddPlayerController(APlayerController* NewPlayer, UUserWidget* PlayerHUDWidget);

protected:
	// Parent Class Function
	virtual void BeginPlay() override;

private:
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UUserWidget>> mPlayerHUDs;
};
