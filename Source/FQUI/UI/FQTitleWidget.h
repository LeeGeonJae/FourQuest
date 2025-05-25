// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQTitleWidget.generated.h"

UCLASS()
class FQUI_API UFQTitleWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQTitleWidget();

	// Input Function
	void WidgetInput(EWidgetInputType InputType);

protected:
	// Parent Function
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Common Function
	void MoveIndex(EWidgetInputType InputType);
	void SelectButton();
	void CancelButton();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation)
	TArray<TObjectPtr<UTexture2D>> mSoulAnimations;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCurrentSelect;

private:
	float mElapsedTime;
	uint8 mCurrentFrameIndex;

	uint8 mCurrentSelectIndex;
	uint8 mMaxSelectIndex;
};
