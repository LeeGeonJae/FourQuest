// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMainCenterCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components\BoxComponent.h"


AFQMainCenterCamera::AFQMainCenterCamera()
{
    // 카메라 줌인 줌아웃
    mCurrentCameraZoom = 2000.f;
    mCameraZoomMin = FVector(1920.f, 1080.f, 2000.f);
    mCameraZoomMax = FVector(3840.f, 2160.f, 3500.f);

    // 카메라 줌인 줌아웃 판정 범위
    mBaseRange = FVector2D(800.f, 1500.f);
    mRangeMaxScale = 1.25f;
    mRangeMinScale = 0.75f;

    // 카메라 붐 컴포넌트
    mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    mCameraBoom->SetupAttachment(RootComponent);
    mCameraBoom->TargetArmLength = mCurrentCameraZoom;
    mCameraBoom->bUsePawnControlRotation = true;
    
    // 카메라 컴포넌트
    mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    mFollowCamera->SetupAttachment(mCameraBoom, TEXT("SpringEndpoint"));
    mFollowCamera->bUsePawnControlRotation = false;

    // 틱 함수 On
    PrimaryActorTick.bCanEverTick = true;

    // 카메라 이동 블렌더 값
    mCameraBlendValue = 0.5f;

    SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f));
    mCurrentCameraLocation = GetActorLocation();
}

void AFQMainCenterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CameraMoveControl(DeltaTime);
    CameraZoomControl(DeltaTime);
}

void AFQMainCenterCamera::CameraMoveControl(float DeltaTime)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    if (PlayerControllers.Num() == 0)
    {
        return;
    }

    FVector Sum = FVector::ZeroVector;
    int32 Count = 0;
    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            Sum += PC->GetPawn()->GetActorLocation();
            Count++;
        }
    }

    FVector CenterLocation = Count > 0 ? (Sum / Count) : FVector::ZeroVector;

    FVector TargetLocation = FVector(CenterLocation.X, CenterLocation.Y, mCurrentCameraLocation.Z); // Z는 유지
    FVector NewLocation = FMath::VInterpTo(mCurrentCameraLocation, TargetLocation, DeltaTime, mCameraBlendValue);
    SetActorLocation(FVector(NewLocation.X, NewLocation.Y, mCurrentCameraLocation.Z));
    mCurrentCameraLocation = NewLocation;
}

void AFQMainCenterCamera::CameraZoomControl(float DeltaTime)
{
    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    // 플레이어의 수가 1명이면 반환
    if (PlayerControllers.Num() <= 1)
    {
        return;
    }

    // 판정 범위 계산
    FVector2D CurrentRange;
    if (mCurrentCameraZoom > mCameraZoomMin.Z)
    {
        float ZoomAlpha = (mCurrentCameraZoom - mCameraZoomMin.Z) / (mCameraZoomMax.Z - mCameraZoomMin.Z);
        ZoomAlpha = FMath::Clamp(ZoomAlpha, 0.0f, 1.0f);

        float RangeScale = FMath::Lerp(mRangeMinScale, mRangeMaxScale, ZoomAlpha);

        CurrentRange = mBaseRange * RangeScale;
    }
    else
    {
        CurrentRange = mBaseRange * mRangeMinScale;
    }
    
    // 디버그 체크
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), FVector(CurrentRange.X, CurrentRange.Y, 0.f), FColor::Yellow);
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), FVector(CurrentRange.X * mRangeMinScale, CurrentRange.Y * mRangeMinScale, 0.f), FColor::Green);
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), FVector(CurrentRange.X * mRangeMaxScale, CurrentRange.Y * mRangeMaxScale, 0.f), FColor::Red);

    // 플레이어와 카메라 간격 계산
    float PlayerDistanceX = 0.0f;
    float PlayerDistanceY = 0.0f;
    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
            PlayerDistanceX = FMath::Max(PlayerDistanceX, FMath::Abs(mCurrentCameraLocation.X - PlayerLocation.X));
            PlayerDistanceY = FMath::Max(PlayerDistanceY, FMath::Abs(mCurrentCameraLocation.Y - PlayerLocation.Y));
        }
    }

    // 줌 인, 줌 아웃
    if (PlayerDistanceX >= (CurrentRange.X * mRangeMaxScale) || PlayerDistanceY >= (CurrentRange.Y * mRangeMaxScale))
    {
        mCurrentCameraZoom = FMath::FInterpTo(mCurrentCameraZoom, mCameraZoomMax.Z, DeltaTime, 0.1f);
        mCameraBoom->TargetArmLength = mCurrentCameraZoom;

        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] CameraZoomValue : %f"), __LINE__, mCurrentCameraZoom);
        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] PlayerDistance : (%f, %f), CameraZoomDistance : (%f, %f)"), __LINE__, PlayerDistanceX, PlayerDistanceY, CurrentRange.X * mRangeMaxScale, CurrentRange.Y * mRangeMaxScale);
    }
    else if (PlayerDistanceX <= (CurrentRange.X * mRangeMinScale) || PlayerDistanceY <= (CurrentRange.Y * mRangeMinScale))
    {
        mCurrentCameraZoom = FMath::FInterpTo(mCurrentCameraZoom, mCameraZoomMin.Z, DeltaTime, 0.1f);
        mCameraBoom->TargetArmLength = mCurrentCameraZoom;

        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] CameraZoomValue : %f"), __LINE__, mCurrentCameraZoom);
        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] PlayerDistance : (%f, %f), CameraZoomDistance : (%f, %f)"), __LINE__, PlayerDistanceX, PlayerDistanceY, CurrentRange.X * mRangeMinScale, CurrentRange.Y * mRangeMinScale);
    }
}

