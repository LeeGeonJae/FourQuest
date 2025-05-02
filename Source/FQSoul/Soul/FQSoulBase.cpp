// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "FQSoul\Data\FQSoulDataAsset.h"
#include "FQGameCore\Armour\FQArmourInterface.h"

// Sets default values
AFQSoulBase::AFQSoulBase()
{
	// Dash
	mDashDirection = FVector();
	mbIsDashing = false;

	// Armour
	mbIsPressedArmourChange = false;
	mArmourChangeTimer = 0.f;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapEnd);
	//GetCapsuleComponent()->SetCollisionProfileName();

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// CameraBoom
	mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	mCameraBoom->SetupAttachment(RootComponent);
	mCameraBoom->TargetArmLength = 800.f;
	mCameraBoom->bUsePawnControlRotation = true;
	mCameraBoom->bInheritPitch = false;
	mCameraBoom->bInheritRoll = false;
	mCameraBoom->bInheritYaw = false;
	mCameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	// FollowCamera
	mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	mFollowCamera->SetupAttachment(mCameraBoom, TEXT("SpringEndpoint"));
	mFollowCamera->bUsePawnControlRotation = false;

	// InputMappingContext
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Soul.IMC_Soul'"));
	ensure(InputMappingContextRef.Object);
	if (InputMappingContextRef.Object)
	{
		mDefaultMappingContext = InputMappingContextRef.Object;
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SoulMove.IA_SoulMove'"));
	ensure(InputActionMoveRef.Object);
	if (nullptr != InputActionMoveRef.Object)
	{
		mMoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SoulDash.IA_SoulDash'"));
	ensure(InputActionDashRef.Object);
	if (nullptr != InputActionDashRef.Object)
	{
		mDashAction = InputActionDashRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPickRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SoulSelectArmour.IA_SoulSelectArmour'"));
	ensure(InputActionPickRef.Object);
	if (nullptr != InputActionPickRef.Object)
	{
		mPickAction = InputActionPickRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCancelRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SoulCancelArmour.IA_SoulCancelArmour'"));
	ensure(InputActionCancelRef.Object);
	if (nullptr != InputActionCancelRef.Object)
	{
		mCancelAction = InputActionCancelRef.Object;
	}

	// Data
	static ConstructorHelpers::FObjectFinder<UFQSoulDataAsset> SoulDataAssetRef(TEXT("/Script/FQSoul.FQSoulDataAsset'/Game/Data/DA_SoulData.DA_SoulData'"));
	ensure(SoulDataAssetRef.Object);
	if (nullptr != SoulDataAssetRef.Object)
	{
		mSoulDataAsset = SoulDataAssetRef.Object;
	}
}

FTransform AFQSoulBase::GetActorTransform() const
{
	return GetTransform();
}

void AFQSoulBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;

	SetCharacterControl();
}

void AFQSoulBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mDashCoolTimer -= DeltaTime;

	// 대쉬
	if (mSoulDataAsset && mbIsDashing)
	{
		AddMovementInput(mDashDirection, mSoulDataAsset->mDashSpeed * DeltaTime);

		mDashTimer -= DeltaTime;
		if (mDashTimer <= 0.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = 2048.f;
			mDashCoolTimer = mSoulDataAsset->mDashCoolTime;
			mbIsDashing = false;
		}
	}

	// 갑옷
	ChangeArmour(DeltaTime);
}

void AFQSoulBase::SetCharacterControl()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mDefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(mDefaultMappingContext);
	}
}

void AFQSoulBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &AFQSoulBase::Move);
	EnhancedInputComponent->BindAction(mPickAction, ETriggerEvent::Triggered, this, &AFQSoulBase::PressedPickButton);
	EnhancedInputComponent->BindAction(mCancelAction, ETriggerEvent::Triggered, this, &AFQSoulBase::CancelChangeArmour);
	EnhancedInputComponent->BindAction(mDashAction, ETriggerEvent::Triggered, this, &AFQSoulBase::StartDash);
}

void AFQSoulBase::Move(const FInputActionValue& Value)
{
	if (mbIsDashing)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AFQSoulBase::ChangeArmour(float DeltaTime)
{
	// 예외처리
	if (!mbIsPressedArmourChange || mArmours.Num() == 0 || mCurrentArmour == nullptr)
	{
		return;
	}

	// 현재 가장 가까운 아머가 버튼을 눌렀을 때, 가장 가까운 갑옷이 아닌 경우 리셋
	if (mCurrentArmour != CheckNearArmour())
	{
		CancelChangeArmour();
		return;
	}

	// 시간 체크
	mArmourChangeTimer -= DeltaTime;
	if (mArmourChangeTimer > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("ChangedArmour Time : %f"), mArmourChangeTimer);
		return;
	}

	// 갑옷 타입에 따른 구현
	if (mCurrentArmour->GetArmourType() == EArmourType::Warrior)
	{
		UE_LOG(LogTemp, Log, TEXT("Soul Pick Armour : Warrior"));
	}
	else if (mCurrentArmour->GetArmourType() == EArmourType::Magic)
	{
		UE_LOG(LogTemp, Log, TEXT("Soul Pick Armour : Magic"));
	}

	mCurrentArmour->PickArmour();
	CancelChangeArmour();
	UE_LOG(LogTemp, Log, TEXT("Armours Container Size : %d"), mArmours.Num());
}

void AFQSoulBase::StartDash()
{
	if (mSoulDataAsset && !mbIsDashing && mDashCoolTimer <= 0)
	{
		mbIsDashing = true;
		mDashTimer = mSoulDataAsset->mDashDuration;
		GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = mSoulDataAsset->mDashSpeed * 2;

		mDashDirection = GetLastMovementInputVector().GetSafeNormal();
		if (mDashDirection.IsZero())
		{
			mDashDirection = GetActorForwardVector(); // 입력 없으면 정면
		}
	}
}

void AFQSoulBase::PressedPickButton()
{
	UE_LOG(LogTemp, Log, TEXT("[Function] PressedPickButton"));
	mbIsPressedArmourChange = true;
	mArmourChangeTimer = mSoulDataAsset->mArmourDelayTime;
	mCurrentArmour = CheckNearArmour();
}

void AFQSoulBase::CancelChangeArmour()
{
	UE_LOG(LogTemp, Log, TEXT("[Function] CancelChangeArmour"));
	mbIsPressedArmourChange = false;
	mCurrentArmour = nullptr;
}

IFQArmourInterface* AFQSoulBase::CheckNearArmour()
{
	IFQArmourInterface* MinDistanceArmour = nullptr;
	FVector SoulLocation = GetActorLocation();

	for (const auto& Armour : mArmours)
	{
		if (MinDistanceArmour == nullptr)
		{
			MinDistanceArmour = Armour.Value;
		}
		else
		{
			FVector CurrentMinArmourLocation = MinDistanceArmour->GetActorTransform().GetLocation();
			FVector TargetArmourLocation = Armour.Value->GetActorTransform().GetLocation();

			float CurrentMinArmourDis = FVector::Dist(SoulLocation, CurrentMinArmourLocation);
			float TargetArmourDis = FVector::Dist(SoulLocation, TargetArmourLocation);

			if (TargetArmourDis < CurrentMinArmourDis)
			{
				MinDistanceArmour = MinDistanceArmour = Armour.Value;
			}
		}
	}

	return MinDistanceArmour;
}

void AFQSoulBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IFQArmourInterface* OverlappingArmour = Cast<IFQArmourInterface>(OtherActor);
	if (OverlappingArmour)
	{
		mArmours.Add(OtherActor->GetName(), OverlappingArmour);
	}
}

void AFQSoulBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IFQArmourInterface* OverlappingArmour = Cast<IFQArmourInterface>(OtherActor);
	if (OverlappingArmour)
	{
		mArmours.FindAndRemoveChecked(OtherActor->GetName());
	}
}