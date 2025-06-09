// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFQPlayerBase::AFQPlayerBase()
{
	mMoveDirection = FVector::Zero();

	// Pawn
	// 컨트롤러에 의한 카메라 회전 방지
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// 캡슐 컴포넌트 크기 및 콜리전 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCollision"));
	
	// Input
	mDefaultSpeed = 200.0f;
	
	// Mesh
	// 캐릭터 메쉬 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->bReceivesDecals = false;

	// Effect
	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SoulEffect"));
	mEffect->SetupAttachment(RootComponent);
	mEffect->SetAutoActivate(true);

	mHitState = EHitState::None;

	mbIsPressedX = false;
	mbIsPressedA = false;
}

void AFQPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::Move);
}

void AFQPlayerBase::SetHitReacting(bool HitReacting)
{
	if (HitReacting)
	{
		mHitState = EHitState::HitReacting;
	}
}

void AFQPlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float AFQPlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.f)
	{
		return 0.f;
	}
	UE_LOG(LogTemp,Warning,TEXT("Take Damage"))
	return ActualDamage;
}

void AFQPlayerBase::ApplyDamageToTarget(float DamageAmount, AActor* Target)
{
	UGameplayStatics::ApplyDamage(Target, DamageAmount, GetController(), this, UDamageType::StaticClass());
}

void AFQPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context 적용
	SetInputMappingContext();

	// Character Movement 기본 설정 적용
	SetMovement();
}

void AFQPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector2D NormalizedVector = MovementVector.GetSafeNormal();
	mMoveDirection = FVector(NormalizedVector.X, NormalizedVector.Y, 0.0f);

	ProcessInputMovement();

	if (!CanMove())
	{
		return;
	}

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AFQPlayerBase::SetInputMappingContext()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mDefaultMappingContext, 0);
	}
}

void AFQPlayerBase::SetMovement()
{
	// Movement
	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
}
