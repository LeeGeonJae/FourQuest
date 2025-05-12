// Fill out your copyright notice in the Description page of Project Settings.


#include "FQUserWidget.h"

void UFQUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (mOwningActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerActor Is Not Vaild"));
	}
}
