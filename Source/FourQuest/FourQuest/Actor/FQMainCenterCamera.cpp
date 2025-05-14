// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMainCenterCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components\BoxComponent.h"
#include "FQCameraDataAsset.h"


AFQMainCenterCamera::AFQMainCenterCamera()
{
    mCurrentCameraZoomValue = 2000.f;

    // 에셋 데이터
    ConstructorHelpers::FObjectFinder<UFQCameraDataAsset> DataAssetRef(TEXT("/Script/FourQuest.FQCameraDataAsset'/Game/Data/DA_CameraData.DA_CameraData'"));
    if (DataAssetRef.Succeeded()) 
    {
        mCameraDataAsset = DataAssetRef.Object;
    }

    // 카메라 붐 컴포넌트
    mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    mCameraBoom->SetupAttachment(RootComponent);
    mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;
    mCameraBoom->bUsePawnControlRotation = true;
    
    // 카메라 컴포넌트
    mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    mFollowCamera->SetupAttachment(mCameraBoom, TEXT("SpringEndpoint"));
    mFollowCamera->bUsePawnControlRotation = false;

    // 틱 함수 On
    PrimaryActorTick.bCanEverTick = true;

    SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f));
    mCurrentCameraLocation = GetActorLocation();
}

void AFQMainCenterCamera::BeginPlay()
{
    Super::BeginPlay();

    mCurrentCameraZoomValue = mCameraDataAsset->mCameraZoomMin;
    mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;

    mFollowCamera->bDrawFrustumAllowed = true;
    mFollowCamera->OverrideFrustumColor(FColor::Green);
}

void AFQMainCenterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (mCameraDataAsset)
    {
        CameraMoveControl(DeltaTime);           // 카메라 이동
        CameraZoomControl(DeltaTime);           // 카메라 줌 컨트롤
        CameraWallCollisionUpdate(DeltaTime);   // 카메라 벽 (시야 범위의 경계선 콜리전 설치)
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQMainCenterCamera %d] Is Not Find UFQCameraDataAsset"), __LINE__);
    }
}

void AFQMainCenterCamera::CameraMoveControl(float DeltaTime)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // 플레이어 컨트롤러 확인
    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    if (PlayerControllers.Num() == 0)
    {
        return;
    }

    // 플레이어들의 위치 확인
    FVector Sum = FVector::ZeroVector;
    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            Sum += PC->GetPawn()->GetActorLocation();
        }
    }

    // 카메라 위치 조정 (블렌더 처리)
    int32 Count = PlayerControllers.Num();
    FVector CenterLocation = Count > 0 ? (Sum / Count) : FVector::ZeroVector;

    FVector TargetLocation = FVector(CenterLocation.X, CenterLocation.Y, mCurrentCameraLocation.Z); // Z는 유지
    FVector NewLocation = FMath::VInterpTo(mCurrentCameraLocation, TargetLocation, DeltaTime, mCameraDataAsset->mCameraBlendValue);
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
    if (mCurrentCameraZoomValue > mCameraDataAsset->mCameraZoomMin)
    {
        float ZoomAlpha = (mCurrentCameraZoomValue - mCameraDataAsset->mCameraZoomMin) / (mCameraDataAsset->mCameraZoomMax - mCameraDataAsset->mCameraZoomMin);
        ZoomAlpha = FMath::Clamp(ZoomAlpha, 0.0f, 1.0f);

        float RangeScale = FMath::Lerp(mCameraDataAsset->mCheckRangeMinScale, mCameraDataAsset->mCheckRangeMaxScale, ZoomAlpha);
        CurrentRange = mCameraDataAsset->mBaseCheckRange * RangeScale;
    }
    else
    {
        CurrentRange = mCameraDataAsset->mBaseCheckRange * mCameraDataAsset->mCheckRangeMinScale;
    }
    
    // 디버그 체크
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), FVector(CurrentRange.X, CurrentRange.Y, 0.f), FColor::Yellow);
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), 
        FVector(CurrentRange.X * mCameraDataAsset->mCheckRangeMinScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMinScale, 0.f), FColor::Green);
    DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), 
        FVector(CurrentRange.X * mCameraDataAsset->mCheckRangeMaxScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMaxScale, 0.f), FColor::Red);

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
    if (PlayerDistanceX >= (CurrentRange.X * mCameraDataAsset->mCheckRangeMaxScale) || PlayerDistanceY >= (CurrentRange.Y * mCameraDataAsset->mCheckRangeMaxScale))
    {
        mCurrentCameraZoomValue = FMath::FInterpTo(mCurrentCameraZoomValue, mCameraDataAsset->mCameraZoomMax, DeltaTime, mCameraDataAsset->CameraZoomSpeed);
        mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;

        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] CameraZoomValue : %f"), __LINE__, mCurrentCameraZoomValue);
        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] PlayerDistance : (%f, %f), CameraZoomDistance : (%f, %f)"), __LINE__, PlayerDistanceX, PlayerDistanceY, CurrentRange.X * mCameraDataAsset->mCheckRangeMaxScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMaxScale);
    }
    else if (PlayerDistanceX <= (CurrentRange.X * mCameraDataAsset->mCheckRangeMinScale) || PlayerDistanceY <= (CurrentRange.Y * mCameraDataAsset->mCheckRangeMinScale))
    {
        mCurrentCameraZoomValue = FMath::FInterpTo(mCurrentCameraZoomValue, mCameraDataAsset->mCameraZoomMin, DeltaTime, mCameraDataAsset->CameraZoomSpeed);
        mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;

        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] CameraZoomValue : %f"), __LINE__, mCurrentCameraZoomValue);
        UE_LOG(LogTemp, Log, TEXT("[AFQMainCenterCamera %d] PlayerDistance : (%f, %f), CameraZoomDistance : (%f, %f)"), __LINE__, PlayerDistanceX, PlayerDistanceY, CurrentRange.X * mCameraDataAsset->mCheckRangeMinScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMinScale);
    }
}

void AFQMainCenterCamera::CameraWallCollisionUpdate(float DeltaTime)
{
    RaycastFrustumEdges();
}

void AFQMainCenterCamera::RaycastFrustumEdges()
{
    // 카메라 뷰 정보 가져오기
    FMinimalViewInfo CameraView;
    mFollowCamera->GetCameraView(0.f, CameraView);

    // 카메라 위치 & 회전
    FVector CameraLocation = mFollowCamera->GetComponentLocation();
    FRotator CameraRotation = mFollowCamera->GetComponentRotation();

    // FOV 및 종횡비
    float HalfFOV = FMath::DegreesToRadians(CameraView.FOV) * 0.5f;
    float AspectRatio = CameraView.AspectRatio;

    // 방향 벡터 계산
    FVector CameraForward = CameraRotation.Vector();
    FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
    FVector CameraUp = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Z);

    FVector TraceTopDirection = (CameraForward + FMath::Tan(HalfFOV) * CameraUp).GetSafeNormal();
    FVector TraceBottomDirection = (CameraForward - FMath::Tan(HalfFOV) * CameraUp).GetSafeNormal();
    FVector TraceRigetDirection = (CameraForward + FMath::Tan(HalfFOV) * AspectRatio * CameraRight).GetSafeNormal();
    FVector TraceLeftDirection = (CameraForward - FMath::Tan(HalfFOV) * AspectRatio * CameraRight).GetSafeNormal();

    // 디버그 및 트레이스
    float TraceLength = 10000.0f;

    auto ShootRay = [&](FVector Direction, FColor DebugColor)
        {
            FVector End = CameraLocation + Direction * TraceLength;
            DrawDebugLine(GetWorld(), CameraLocation, End, DebugColor, false, 2.0f, 0, 2.0f);

            FHitResult HitResult;
            GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility);
            if (HitResult.bBlockingHit)
            {
                DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.0f, 12, DebugColor, false, 2.0f);
            }
        };

    ShootRay(TraceTopDirection, FColor::Red);
    ShootRay(TraceBottomDirection, FColor::Green);
    ShootRay(TraceRigetDirection, FColor::Blue);
    ShootRay(TraceLeftDirection, FColor::Yellow);
}

