// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFQPlayerBase::AFQPlayerBase()
{
	// Pawn
	// 컨트롤러에 의한 카메라 회전 방지
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// 캡슐 컴포넌트 크기 및 콜리전 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	
	// Movement
	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	// 캐릭터 메쉬 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// Camera
	mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	mCameraBoom->SetupAttachment(RootComponent);
	mCameraBoom->TargetArmLength = 400.0f;
	mCameraBoom->bUsePawnControlRotation = true;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mCameraBoom, USpringArmComponent::SocketName);
	mCamera->bUsePawnControlRotation = false;

	// Input
	// Dash 기본 설정
	mDashSpeed = 500.0f;
	mDashDuration = 0.5f;
	mDashCoolTime = 1.0f;
	mbCanDash = false;
	mbIsDashing = false;
}

FTransform AFQPlayerBase::GetTransform() const
{
	return GetTransform();
}

void AFQPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::Move);
	EnhancedInputComponent->BindAction(mDashAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::Dash);
}

void AFQPlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (mbIsDashing)
	{
		AddMovementInput(mDashDirection, mDashSpeed * DeltaSeconds);
	}
}

void AFQPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Input
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mDefaultMappingContext, 0);
	}
}

void AFQPlayerBase::Move(const FInputActionValue& Value)
{
	if (mbIsDashing)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AFQPlayerBase::Dash()
{
	mbCanDash = true;

	if (mbCanDash)
	{
		StartDash();
	}
}

void AFQPlayerBase::StartDash()
{
	if (!mbIsDashing)
	{
		mbCanDash = false;
		mbIsDashing = true;

		mDashDirection = GetLastMovementInputVector().GetSafeNormal();
		if (mDashDirection.IsZero())
		{
			mDashDirection = GetActorForwardVector(); 
		}

		GetCharacterMovement()->MaxWalkSpeed = mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = mDashSpeed * 2;

		GetWorld()->GetTimerManager().SetTimer(mDashTimer, this, &AFQPlayerBase::EndDash, mDashDuration, false);
		GetWorld()->GetTimerManager().SetTimer(mDashCoolTimer, this, &AFQPlayerBase::ResetDash, mDashCoolTime, false);
	}
}

void AFQPlayerBase::EndDash()
{
	mbIsDashing = false;

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void AFQPlayerBase::ResetDash()
{
	mbCanDash = true;
}
