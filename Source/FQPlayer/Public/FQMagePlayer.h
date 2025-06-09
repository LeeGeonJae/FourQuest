// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerBase.h"
#include "FQMagePlayer.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API AFQMagePlayer : public AFQPlayerBase
{
	GENERATED_BODY()

public:
	// Default
	AFQMagePlayer();
	virtual void Tick(float DeltaSeconds) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Animation
	virtual void ProcessNextSection() override;

	bool IsEnabledExplosionCircle();

	// Projectile Attack
	virtual void EnableAttackVolume() override;
	virtual void DisableAttackVolume() override;

	void CheckProjectileAttackVolume();
	void ProcessProjectileAttack();
	bool ApplyPush(class AActor* AttackableActor);

protected:
	// Default
	virtual void BeginPlay() override;

	// Input
	virtual void SetInputMappingContext() override;
	virtual bool CanMove() override;
	virtual void ProcessInputMovement() override;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UFQMageDataAsset> mMageDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mMageMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mProjectileAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mExplosionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mLaserAction;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAnimInstance> mMageAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mProjectileAttackAnim1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mProjectileAttackAnim2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mExplosionStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mExplosionEndAnim;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMageProjectileAttackState mProjectileAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EComboState mProjectileAttackComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMageExplosionState mExplosionState;

	// Projectile Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mProjectileAttackVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFQMageProjectile> mProjectileClass;

	// Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFQMageCircle> mExplosionCircleClass;

private:
	// Animation
	UFUNCTION()
	void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Projectile Attack
	FVector mLookAtDirection;
	FRotator mLookAtRotation;

	FTimerHandle mProjectileAttackComboTimer;
	FTimerHandle mProjectileAttackCoolTimer;

	void StartProjectileAttack();
	void PressedProjectileAttack();
	void EndProjectileAttack();

	void ResetProjectileAttackCombo();
	void ResetProjectileAttackCoolDown();

	// Explosion
	UPROPERTY()
	TObjectPtr<AFQMageCircle> mExplosionCircle;

	FTimerHandle mExplosionCoolTimer;

	void StartExplosion();
	void ProcessExplosion();
	void EndExplosion();
	void Explosion();
};
