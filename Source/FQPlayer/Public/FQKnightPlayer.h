// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerBase.h"
#include "FQKnightPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API AFQKnightPlayer : public AFQPlayerBase
{
	GENERATED_BODY()
	
public:
	// Default
	AFQKnightPlayer();
	virtual void Tick(float DeltaSeconds) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sword Attack
	virtual void EnableAttackVolume() override;
	virtual void DisableAttackVolume() override;

	// Bash
	void EnableBashVolume();
	void DisableBashVolume();

	// Shield
	void EnableShieldVolume();
	void DisableShieldVolume();
	float GetShieldMoveAngle();
	bool IsHoldShield();

	// Animation
	virtual void SetAnimInstance() override;
	virtual void ProcessNextSection() override;

protected:
	// Default
	virtual void BeginPlay() override;

	// Input
	virtual void SetInputMappingContext() override;
	virtual bool CanMove() override;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UFQKnightDataAsset> mKnightDataAsset;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mKnightMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mBashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mSwordAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mShieldAction;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAnimInstance> mKnightAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mSwordAttackAnim1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mSwordAttackAnim2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mSwordAttackAnim3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mBashAnim;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EKnightSwordAttackState mSwordAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EComboState mSwordAttackComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EKnightShieldState mShieldState;

	// Sword Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[X] 충돌 콜리전"))
	TObjectPtr<class UBoxComponent> mSwordAttackVolume;

	// Bash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[A] 충돌 콜리전"))
	TObjectPtr<class UBoxComponent> mBashVolume;

	// Shield
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mShieldUpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mShieldDownAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), meta = (ToolTip = "[R] 충돌 콜리전"))
	TObjectPtr<class UBoxComponent> mShieldVolume;

private:
	void ApplyDamage(class AActor* AttackableActor);
	void ApplyPush(class AActor* AttackableActor, float Strength);

	// Bash
	FVector mBashDirection;
	float mBashElapsedTime;

	// Bash할 수 있는 상태인지 확인하는 플래그
	uint8 mbCanBash : 1;
	// Bash를 하는 중인지 확인하는 플래그
	uint8 mbIsBashing : 1;

	FTimerHandle mBashCoolTimer;

	void StartBash();
	void Bash();
	void EndBash();
	void ResetBash();
	void CheckBashVolume();

	UFUNCTION()
	void OnBashVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sword Attack
	FTimerHandle mSwordAttackComboTimer;
	FTimerHandle mSwordAttackCoolTimer;

	void ResetSwordAttackCombo();
	void ResetSwordAttackCoolDown();
	void StartSwordAttack();
	void EndSwordAttack();
	void PressedSwordAttack();
	void CheckSwordAttackVolume();

	// Shield
	uint8 mbIsShielding : 1;
	float mShieldMoveAngle;
	FTimerHandle mShieldCoolTimer;
	FRotator mShieldRotation;

	void StartShieldMove();
	void ShieldMove(const FInputActionValue& Value);
	void EndShieldMove();
	void CheckShiedVolume();

	UFUNCTION()
	void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnShieldVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
