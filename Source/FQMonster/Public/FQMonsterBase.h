

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQMonsterDataAsset.h"
#include "FQGameCore/Player/FQPlayerAttackableInterface.h"
#include "FQMonsterBase.generated.h"

class AFQMonsterManager;
class UBoxComponent;

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
class FQMONSTER_API AFQMonsterBase : public ACharacter, public IFQPlayerAttackableInterface
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

	virtual void Attack(AActor* Actor);

	void ManagerSetTargetActor(AActor* Actor);
	
	FName GetRandomSectionName();

	void ApplyDamageToTarget();

	void SetCollisionEnabled(bool CollisionEnabled);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Stat")
	float mCurrentHP;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	UFQMonsterDataAsset* mMonsterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterGroup")
	FName mGroupID=TEXT("None");

	FVector mSpawnedLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	EMonsterState mMonsterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
	UAnimMontage* mAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
	AActor* mTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UBoxComponent* mAttackBox;

protected:
	AFQMonsterManager* Manager;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mPushCoolTime;

	virtual void TakeDamageByPlayer(AActor* Target, float Damage) override;
	virtual void TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength) override;

private:
	uint8 mbCanPush : 1;

	FTimerHandle mPushCoolTimer;


};
