// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	// 캐릭터 메쉬 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
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
	EnhancedInputComponent->BindAction(mInteractiveAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::Interaction);
}

void AFQPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mDefaultMappingContext, 0);
	}
}

void AFQPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("MovementVector : %f, %f"), MovementVector.X, MovementVector.Y);
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AFQPlayerBase::Dash(const FInputActionValue& Value)
{
}

void AFQPlayerBase::Interaction(const FInputActionValue& Value)
{
}
