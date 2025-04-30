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

// Sets default values
AFQSoulBase::AFQSoulBase()
{
	mDashDirection = FVector();
	mbIsDashing = false;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
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

	// Data
	static ConstructorHelpers::FObjectFinder<UFQSoulDataAsset> SoulDataAssetRef(TEXT("/Script/FQSoul.FQSoulDataAsset'/Game/Data/DA_SoulData.DA_SoulData'"));
	ensure(SoulDataAssetRef.Object);
	if (nullptr != SoulDataAssetRef.Object)
	{
		mSoulDataAsset = SoulDataAssetRef.Object;
	}
}

FTransform AFQSoulBase::GetTransform() const
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

	if (mSoulDataAsset && mbIsDashing)
	{
		AddMovementInput(mDashDirection, mSoulDataAsset->mDashSpeed * DeltaTime);

		UE_LOG(LogTemp, Log, TEXT("Current Speed : %f"), GetCharacterMovement()->GetCurrentAcceleration().Length());
		mDashTimer -= DeltaTime;
		if (mDashTimer <= 0.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = 2048.f;
			mbIsDashing = false;
		}
	}
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
	EnhancedInputComponent->BindAction(mPickAction, ETriggerEvent::Triggered, this, &AFQSoulBase::ChangeArmour);
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

	UE_LOG(LogTemp, Log, TEXT("[Input] X : %f, Y : %f"), MovementVector.X, MovementVector.Y);

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AFQSoulBase::ChangeArmour()
{

}

void AFQSoulBase::StartDash()
{
	if (mSoulDataAsset && !mbIsDashing)
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
