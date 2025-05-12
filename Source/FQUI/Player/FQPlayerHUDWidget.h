// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
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

protected:
    virtual void NativeConstruct() override;

protected:
    // 타입별 텍스처 (에디터에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    UTexture2D* mKnightSoulTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
    UTexture2D* mMagicSoulTexture;
};
