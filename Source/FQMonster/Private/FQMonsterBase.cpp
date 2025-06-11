


#include "FQMonsterBase.h"
#include "FQMonsterManager.h"
#include "FQMonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AFQMonsterBase::AFQMonsterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mAttackMontage = nullptr;
	mTargetActor = nullptr;

	mbCanPush = true;
	mPushCoolTime = 0.0f;

	mAttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collider"));
	mAttackBox->SetupAttachment(RootComponent);
	mAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCollision"));

	GetMesh()->bReceivesDecals = false;
}

// Called when the game starts or when spawned
void AFQMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	mSpawnedLocation = GetActorLocation();

	if (mMonsterDataAsset)
	{
		mCurrentHP = mMonsterDataAsset->mMaxHP;
	}
	
	for (TActorIterator<AFQMonsterManager> It(GetWorld()); It; ++It)
	{
		Manager = *It;
		if(mGroupID==TEXT("None"))
			It->RegisterGroup(TEXT("Default"), this);
		else
			It->RegisterGroup(mGroupID, this);
		break;
	}
	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &AFQMonsterBase::OnHitMontageEnded);
			UE_LOG(LogTemp, Warning, TEXT("Delegate 등록"))
		}
	}
}

// Called every frame
void AFQMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFQMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFQMonsterBase::Attack(AActor* Actor)
{
	if (mAttackMontage)
	{
		FName SectionName = GetRandomSectionName();
		PlayAnimMontage(mAttackMontage, 1.0f, SectionName);
	}
}

void AFQMonsterBase::ManagerSetTargetActor(AActor* Actor)
{
	if (!mTargetActor)
	{
		mTargetActor = Actor;
	}
	Manager->SetTargetActor(mGroupID, Actor);
}


FName AFQMonsterBase::GetRandomSectionName()
{
	const TArray<FName> Sections = { TEXT("Attack1"), TEXT("Attack2"), TEXT("Attack3"), TEXT("Attack4"), TEXT("Attack5") };
	int32 Index = FMath::RandRange(0, Sections.Num() - 1);
	return Sections[Index];
}

void AFQMonsterBase::ApplyDamageToTarget()
{
	TArray<AActor*> Overlapped;
	mAttackBox->GetOverlappingActors(Overlapped, AActor::StaticClass());

	for (AActor* Target : Overlapped)
	{
		if (Target && Target != this)
		{
			UGameplayStatics::ApplyDamage(Target, mMonsterDataAsset->mAttackPower, GetController(), this, UDamageType::StaticClass());
		}
	}
	
}

void AFQMonsterBase::SetCollisionEnabled(bool CollisionEnabled)
{
	if (CollisionEnabled)
		mAttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		mAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AFQMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		AIC->ChangeState(EMonsterState::Hit);
		Hit();
		AIC->ChangeTargetActor(DamageCauser);
	}

	return DamageAmount;
}

void AFQMonsterBase::Hit()
{
	if (mHitMontage)
	{
		const TArray<FName> Sections = { TEXT("Hit1"), TEXT("Hit2") };
		int32 Index = FMath::RandRange(0, Sections.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Hit Animation"))
		PlayAnimMontage(mHitMontage,1.0f, Sections[Index]);
	}
}

void AFQMonsterBase::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Animation 종료"))
	if (Montage == mHitMontage)
	{
		AFQMonsterAIController* AIC = GetController<AFQMonsterAIController>();
		if (AIC)
		{
			AIC->ChangeState(EMonsterState::Chase);
			UE_LOG(LogTemp, Warning, TEXT("Hit Animation 종료"))
		}
	}
}

void AFQMonsterBase::TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength)
{
	if (!Target)
	{
		return;
	}

	if (Target != this)
	{
		return;
	}

	if (!mbCanPush)
	{
		return;
	}

	// 밀리는 힘이 중복으로 적용되지 않도록 쿨타임 설정
	mbCanPush = false;
	GetWorld()->GetTimerManager().SetTimer(mPushCoolTimer, [this]() { mbCanPush = true; }, mPushCoolTime, false);

	LaunchCharacter(Direction * Strength, true, true);
}


