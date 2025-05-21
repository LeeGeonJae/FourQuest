// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
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

	// Input Function
	void WidgetInput(EWidgetInputType InputType);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Common Function
	void MoveIndex(EWidgetInputType InputType);
	void SelectButton();
	void CancelButton();

private:

};
