// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQSoulComplateUI.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulComplateUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQSoulComplateUI();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> m3DSoulAnimation;
};
