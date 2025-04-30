// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values
AFQSoulBase::AFQSoulBase()
{
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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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


	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Props/Soul/Input/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		mMoveAction = InputActionMoveRef.Object;
	}

	// InputMappingContext
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Props/Soul/Input/IMC_Quater.IMC_Quater'"));
	if (InputMappingContextRef.Object)
	{
		mDefaultMappingContext = InputMappingContextRef.Object;
	}
}

FTransform AFQSoulBase::GetTransform() const
{
	return GetTransform();
}

void AFQSoulBase::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl();
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

// Called to bind functionality to input
void AFQSoulBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent"));

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &AFQSoulBase::Move);
}

void AFQSoulBase::Move(const FInputActionValue& Value)
{
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

