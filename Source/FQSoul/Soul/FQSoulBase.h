// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "InputActionValue.h"
#include "FQGameCore\Soul/CommonSoul.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	AFQSoulBase();

	// Character
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IFQSoulCharacterInterface
	virtual FTransform GetActorTransform() const override;
	virtual ESoulType GetSoulType() const override;


private:
	void SetCharacterControl();

	void Move(const FInputActionValue& Value);
	void StartDash();
	void SelectInteraction();
	void CancelInteraction();

	// Change Armour
	void CheckArmour(float DeltaTime);
	class IFQArmourInterface* CheckNearArmour();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Input
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mPickAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mCancelAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mDashAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UFQSoulDataAsset> mSoulDataAsset;

	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFQWidgetComponent> mArmourGaugeWidget;

	// Soul
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soul, Meta = (AllowPrivateAccess = "true"))
	ESoulType mSoulType;

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
