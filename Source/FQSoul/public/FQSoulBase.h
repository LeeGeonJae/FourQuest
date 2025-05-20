// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "InputActionValue.h"
#include "FQGameCore\Common.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	AFQSoulBase();

	// Interface Class Function
	virtual FTransform GetActorTransform() const override;

protected:
	// Parent Class Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Common Function
	void SetCharacterControl();

	// Input Funtion
	void Move(const FInputActionValue& Value);
	void StartDash();
	void SelectInteraction();
	void CancelInteraction();
	void AddSoulGauge();		// Test Function

	// Change Armour Function
	void CheckArmour(float DeltaTime);
	class IFQArmourInterface* CheckNearArmour();

	// Collision
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Input
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mPickAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mCancelAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class UFQSoulDataAsset> mSoulDataAsset;

	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFQWidgetComponent> mArmourGaugeWidget;

private:
	// 대시 변수
	uint8 mbIsDashing : 1;
	FVector mDashDirection;
	float	mDashTimer;
	float	mDashCoolTimer;

	// 갑옷 변수
	uint8 mbIsPressedArmourChange : 1;
	float mArmourChangeTimer;

	UPROPERTY()
	TMap<FString, TObjectPtr<UObject>> mArmours;

	UPROPERTY()
	TObjectPtr<UObject> mCurrentArmour;
};
