// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FQGameCore/Player/FQPlayerCharacterInterface.h"
#include "InputActionValue.h"

#include "FQPlayerBase.generated.h"

UCLASS()
class FQPLAYER_API AFQPlayerBase : public ACharacter, public IFQPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQPlayerBase();

	// FQPlayerCharacterInterface
	virtual FTransform GetTransform() const override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mInteractiveAction;

	void Move(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Interaction(const FInputActionValue& Value);
};
