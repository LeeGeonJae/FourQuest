// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "FQGameCore/Player/FQPlayerCharacterInterface.h"
#include "FQPlayer/Public/FQPlayerActionState.h"

#include "FQPlayerBase.generated.h"

UCLASS()
class FQPLAYER_API AFQPlayerBase : public ACharacter, public IFQPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQPlayerBase();

	virtual void Tick(float DeltaSeconds) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHitReacting(bool HitReacting);

	virtual void ProcessNextSection(const FName& SectionName) PURE_VIRTUAL(AFQPlayerBase::ProcessNextSection, );

protected:
	virtual void BeginPlay() override;

	// Setting
	virtual void SetInputMappingContext();

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "기본 속도"))
	float mDefaultSpeed;

	void Move(const FInputActionValue& Value);

	// Effect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> mEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mEffectSystem;

	// Hit 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Knight, Meta = (AllowPrivateAccess = "true"))
	EHitState mHitState;

	// X 버튼을 길게 누르고 있는 상태인지 확인하는 플래그
	uint8 mbIsPressedX : 1;

private:
	
	void SetMovement();

	
};
