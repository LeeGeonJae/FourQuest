// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageCircle.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraComponent.h"

#include "FQPlayer/Public/FQMagePlayer.h"

// Sets default values
AFQMageCircle::AFQMageCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mInitBoxExtent = FVector(128.0f, 128.0f, 32.0f);
	mInitDecalSize = FVector(128.0f, 128.0f, 128.0f);

	mVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	mVolume->SetBoxExtent(mInitBoxExtent);
	RootComponent = mVolume;

	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	mEffect->SetupAttachment(RootComponent);
	mEffect->SetAutoActivate(false);

	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	mDecal->DecalSize = mInitDecalSize;
	mDecal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFQMageCircle::BeginPlay()
{
	Super::BeginPlay();

	mVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mEffect->OnSystemFinished.AddDynamic(this, &AFQMageCircle::OnEffectFinished);
}

// Called every frame
void AFQMageCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFQMageCircle::ActivateEffect()
{
	if (!mEffect)
	{
		return;
	}

	mEffect->Activate();
}

void AFQMageCircle::SetScale(float ScaleFactor)
{
	FVector VolumeExtent = mInitBoxExtent;
	FVector NewVolumeExtent = FVector(VolumeExtent.X * ScaleFactor, VolumeExtent.Y * ScaleFactor, VolumeExtent.Z);
	mVolume->SetBoxExtent(NewVolumeExtent);

	FVector DecalSize = mInitDecalSize;
	FVector NewDecalSize = FVector(DecalSize.X, DecalSize.Y * ScaleFactor, DecalSize.Z * ScaleFactor);
	mDecal->DecalSize = NewDecalSize;
}

void AFQMageCircle::OnEffectFinished(UNiagaraComponent* PSystem)
{
	if (PSystem != mEffect)
	{
		return;
	}

	Destroy();
}


