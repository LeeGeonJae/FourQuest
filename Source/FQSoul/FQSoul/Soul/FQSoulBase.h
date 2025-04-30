// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQSoulBase();

	virtual FTransform GetTransform() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
