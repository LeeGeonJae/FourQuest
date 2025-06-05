// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQRangeMonsterProjectile.generated.h"

UCLASS()
class FQMONSTER_API AFQRangeMonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFQRangeMonsterProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetDamage(float InDamage) { mDamage = InDamage; }

protected:
	UPROPERTY(EditAnywhere)
	class USphereComponent* mCollisionComp;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* mProjectileMovement;

public:
	float mDamage = 0.f;
};
