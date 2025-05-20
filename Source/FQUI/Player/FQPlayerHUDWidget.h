// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQPlayerHUDWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQPlayerHUDWidget();

    void UpdateSoulIcon(ESoulType InSoulType);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    void PlaySoulBurningAnimation(float DeltaTime);
    void LoadingSoulBurningTexture(FString Path, FString FileName, uint32 TextureSize, TArray<TObjectPtr<UTexture2D>>& SoulTextureContainer);

protected:
    // 타입별 텍스처 (에디터에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TObjectPtr<UTexture2D> mKnightSoulTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TObjectPtr<UTexture2D> mMagicSoulTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mBlueSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mYellowSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mRedSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mGreenSoulBurningAnimations;

private:
    float mElapsedTime;
    uint8 mbIsSoulBurning : 1;
    uint8 mCurrentFrameIndex;
    ESoulType mSoulType;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSoulBurning;
};
