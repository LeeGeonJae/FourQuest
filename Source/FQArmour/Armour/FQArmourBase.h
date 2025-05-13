// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Armour/FQArmourInterface.h"
#include "FQArmourBase.generated.h"

UCLASS()
class FQARMOUR_API AFQArmourBase : public AActor, public IFQArmourInterface
{
	GENERATED_BODY()
	
public:	
	AFQArmourBase();

	virtual void Tick(float DeltaTime) override;

	virtual void PickArmour() override;
	virtual void SetNearestArmour(bool IsTrue) override;
	virtual EArmourType GetArmourType() const override;
	virtual FTransform GetActorTransform() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> mTrigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armour, Meta = (AllowPrivateAccess = "true"))
	EArmourType mArmourType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFQWidgetComponent> mArmourWidget;
};
