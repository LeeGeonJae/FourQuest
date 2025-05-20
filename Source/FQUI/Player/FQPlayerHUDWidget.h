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
    void UpdateArmourSkill(EArmourType InArmourType);
    void UpdateSoulGauge(float GaugeValue);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    void PlaySoulBurningAnimation(float DeltaTime);
    void LoadingSoulBurningTexture(FString Path, FString FileName, uint32 TextureSize, TArray<TObjectPtr<UTexture2D>>& SoulTextureContainer);
    void UpdateSoulBurningAnimation(TArray<UTexture2D*> SoulAnimationKey);

protected:
    // 영혼 타입별 이미지 (에디터에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TObjectPtr<UTexture2D> mKnightSoulTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TObjectPtr<UTexture2D> mMagicSoulTexture;
    
    // Soul Burning Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mBlueSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mYellowSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mRedSoulBurningAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    TArray<TObjectPtr<UTexture2D>> mGreenSoulBurningAnimations;

    // Armour Skill 이미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour")
    TMap<EArmourType, TObjectPtr<UTexture2D>> mArmourTypeMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour")
    TMap<EArmourType, TObjectPtr<UTexture2D>> mXSkillMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour")
    TMap<EArmourType, TObjectPtr<UTexture2D>> mASkillMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour")
    TMap<EArmourType, TObjectPtr<UTexture2D>> mRSkillMap;

private:
    float mElapsedTime;
    uint8 mbIsSoulBurning : 1;
    uint8 mCurrentFrameIndex;
    ESoulType mSoulType;

    // SoulBurning
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSoulBurning;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class URadialSlider> mSoulGauge;

    // Armour Skill
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mArmourType;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UHorizontalBox> mSkillBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_X;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_A;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_R;
};
