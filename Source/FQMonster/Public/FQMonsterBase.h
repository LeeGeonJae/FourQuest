

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQMonsterDataAsset.h"
#include "FQMonsterBase.generated.h"

class AFQMonsterManager;

UCLASS()
class FQMONSTER_API AFQMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFQMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ManagerSetTargetActor(AActor* Actor);
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Stat")
	float mCurrentHP;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	UFQMonsterDataAsset* mMonsterDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterGroup")
	FName GroupID=TEXT("None");

	FVector SpawnedLocation;

protected:
	AFQMonsterManager* Manager;
};
