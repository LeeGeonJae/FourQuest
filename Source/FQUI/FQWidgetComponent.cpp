// Fill out your copyright notice in the Description page of Project Settings.


#include "FQWidgetComponent.h"
#include "FQUserWidget.h"

void UFQWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UFQUserWidget* FQUserWidget = Cast<UFQUserWidget>(GetWidget());
	if (FQUserWidget)
	{
		FQUserWidget->SetOwningActor(GetOwner());
	}
}
