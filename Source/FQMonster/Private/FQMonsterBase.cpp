


#include "FQMonsterBase.h"
#include "FQMonsterManager.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFQMonsterBase::AFQMonsterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackMontage = nullptr;
	TargetActor = nullptr;

	mbCanPush = true;
	mPushCoolTime = 0.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCollision"));
}

// Called when the game starts or when spawned
void AFQMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnedLocation = GetActorLocation();

	if (mMonsterDataAsset)
	{
		mCurrentHP = mMonsterDataAsset->mMaxHP;
	}
	
	for (TActorIterator<AFQMonsterManager> It(GetWorld()); It; ++It)
	{
		Manager = *It;
		if(GroupID==TEXT("None"))
			It->RegisterGroup(TEXT("Default"), this);
		else
			It->RegisterGroup(GroupID, this);
		break;
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

void AFQMonsterBase::ManagerSetTargetActor(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
	if (!TargetActor)
	{
		TargetActor = Actor;
	}
	Manager->SetTargetActor(GroupID, Actor);
}

void AFQMonsterBase::AttackPlayer(AActor* Actor)
{
	if (AttackMontage)
	{
		FName SectionName = GetRandomSectionName();
		UE_LOG(LogTemp, Warning, TEXT("%s"),*SectionName.ToString());
		PlayAnimMontage(AttackMontage, 1.0f, SectionName);
	}
}

FName AFQMonsterBase::GetRandomSectionName()
{
	const TArray<FName> Sections = { TEXT("Attack1"), TEXT("Attack2"), TEXT("Attack3"), TEXT("Attack4"), TEXT("Attack5") };
	int32 Index = FMath::RandRange(0, Sections.Num() - 1);
	return Sections[Index];
}

void AFQMonsterBase::ApplyDamageToTarget()
{
	UGameplayStatics::ApplyDamage(TargetActor, mMonsterDataAsset->mAttackPower, GetController(), this, UDamageType::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void AFQMonsterBase::TakeDamageByPlayer(AActor* Target, float Damage)
{
	if (!Target)
	{
		return;
	}

	if (Target != this)
	{
		return;
	}

	// TODO : 플레이어에 의해 데미지를 입고, 몬스터의 체력 깎임
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


