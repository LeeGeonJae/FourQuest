// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FQUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { mOwningActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> mOwningActor;
};
