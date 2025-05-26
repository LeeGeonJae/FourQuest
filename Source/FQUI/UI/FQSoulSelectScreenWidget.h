// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQSoulSelectScreenWidget.generated.h"

UENUM()
enum class ESoulSelectButtonType : uint8
{
	NoSelect = 0,
	SoulSelect,
	SoulComplate,
	End
};

UCLASS()
class FQUI_API UFQSoulSelectScreenWidget : public UFQUserWidget
{
	GENERATED_BODY()

public:
	UFQSoulSelectScreenWidget();

	// Input Function
	void WidgetInput(EWidgetInputType InputType, int32 ControllerId);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Common Function
	void MoveIndex(EWidgetInputType InputType, int32 ControllerId);
	void SelectButton(int32 ControllerId);
	void CancelButton(int32 ControllerId);

private:
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> mNoSelectSoulArr;

	UPROPERTY()
	TArray<TObjectPtr<class UFQSoulSelectUI>> mSoulSelectUIArr;

	UPROPERTY()
	TArray<TObjectPtr<class UFQSoulComplateUI>> mSoulComplateUIArr;
};
