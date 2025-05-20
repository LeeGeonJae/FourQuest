

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQMonsterDataAsset.h"
#include "FQMonsterBase.generated.h"

class AFQMonsterManager;

UENUM()
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Death UMETA(DisplayName = "Death")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterGroup")
	FName GroupID=TEXT("None");

	FVector SpawnedLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	EMonsterState mMonsterState;

protected:
	AFQMonsterManager* Manager;
};
