// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "InputActionValue.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	AFQSoulBase();

	// Character
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IFQSoulCharacterInterface
	virtual FTransform GetTransform() const override;


private:
	void SetCharacterControl();
	void Move(const FInputActionValue& Value);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> mFollowCamera;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;
};
