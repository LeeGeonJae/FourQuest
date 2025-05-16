// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQSoulSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulSelectWidget : public UFQUserWidget
{
	GENERATED_BODY()

public:
	UFQSoulSelectWidget();

protected:
	virtual void NativeConstruct() override;
};
