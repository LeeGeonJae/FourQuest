


#include "FQMonsterBase.h"
#include "FQMonsterManager.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
AFQMonsterBase::AFQMonsterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	Manager->SetTargetActor(GroupID, Actor);
}


