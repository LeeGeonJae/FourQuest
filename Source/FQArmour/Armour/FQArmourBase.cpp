// Fill out your copyright notice in the Description page of Project Settings.


#include "FQArmourBase.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"

// Sets default values
AFQArmourBase::AFQArmourBase()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 함수가 실행되기 위한 bool형 타입 변수
	mArmourType = EArmourType::Warrior;

	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = mTrigger;
	mMesh->SetupAttachment(mTrigger);

	//mTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	mTrigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	mTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFQArmourBase::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Props/Armour/Item_Amor.Item_Amor'"));
	if (BoxMeshRef.Object)
	{
		mMesh->SetStaticMesh(BoxMeshRef.Object);
	}
	mMesh->SetRelativeLocation(FVector(0.0f, -3.5f, 0.0f));
	mMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AFQArmourBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mMesh->AddRelativeRotation(FRotator(0.f, 1.f, 0.f));
}

EArmourType AFQArmourBase::GetArmourType() const
{
	return mArmourType;
}

// Called when the game starts or when spawned
void AFQArmourBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFQArmourBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IFQSoulCharacterInterface* OverlappingPawn = Cast<IFQSoulCharacterInterface>(OtherActor);
	if (OverlappingPawn)
	{
	}
}