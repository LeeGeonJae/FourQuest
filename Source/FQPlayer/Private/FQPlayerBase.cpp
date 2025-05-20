// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraComponent.h"
#include "Animation/AnimMontage.h"

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
	
	// Input
	mDefaultSpeed = 200.0f;
	// Dash 
	mDashSpeed = 500.0f;
	mDashDuration = 0.5f;
	mDashCoolTime = 1.0f;
	mbCanDash = false;
	mbIsDashing = false;

	// Mesh
	// 캐릭터 메쉬 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// Effect
	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SoulEffect"));
	mEffect->SetupAttachment(RootComponent);
	mEffect->SetAutoActivate(false);

	// Knight
	mSwordAttackState = EKnightSwordAttackState::None;
	mSwordAttackComboState = EKnightSwordAttackComboState::None; 
	mSwordAttackCoolTime = 0.0f;
	mSwordAttackWaitTime1 = 0.0f;
	mSwordAttackWaitTime2 = 0.0f;
	
	mHitState = EHitState::None;

	mbIsPressedX = false;
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
	EnhancedInputComponent->BindAction(mDashAction, ETriggerEvent::Started, this, &AFQPlayerBase::Dash);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Started, this, &AFQPlayerBase::StartSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::PressedSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Completed, this, &AFQPlayerBase::EndSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Canceled, this, &AFQPlayerBase::EndSwordAttack);
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

	if (mbIsDashing && mDashCurve)
	{
		mDashElapsedTime += DeltaSeconds;
		float NormalizedTime = FMath::Clamp(mDashElapsedTime / mDashDuration, 0.0f, 1.0f);
		
		mDashDirection = GetLastMovementInputVector().GetSafeNormal();
		if (mDashDirection.IsZero())
		{
			mDashDirection = GetActorForwardVector();
		}

		AddMovementInput(mDashDirection, mDashCurve->GetFloatValue(NormalizedTime));

		if (mDashElapsedTime >= mDashDuration)
		{
			EndDash();
		}
	}
}

void AFQPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context 적용
	SetInputMappingContext();

	// Character Movement 기본 설정 적용
	SetMovement();

	// Effect
	if (mEffectSystem)
	{
		mEffect->SetAsset(mEffectSystem);
		mEffect->Activate();
	}
}

void AFQPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	//UE_LOG(LogTemp, Log, TEXT("Movement : %f, %f"), MovementVector.X, MovementVector.Y);
	
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

void AFQPlayerBase::StartSwordAttack()
{
	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] Function Start"));

	if (mSwordAttackComboState == EKnightSwordAttackComboState::CanBeCombo)
	{
		UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] Combo"));

		mSwordAttackComboState = EKnightSwordAttackComboState::Combo;

		switch (mSwordAttackState)
		{
		case EKnightSwordAttackState::Attack1:
		{
			mSwordAttackState = EKnightSwordAttackState::Attack2;
			ProcessSwordAttack();
		}
		break;
		case EKnightSwordAttackState::Attack2:
		{
			mSwordAttackState = EKnightSwordAttackState::Attack3;
			ProcessSwordAttack();
		}
		break;
		}
	}
	else
	{
		// 공격 입력이 처음으로 들어왔을 때
		if (mSwordAttackState == EKnightSwordAttackState::CoolDown)
		{
			return;
		}

		mSwordAttackState = EKnightSwordAttackState::Attack1;
		ProcessSwordAttack();
	}
}

void AFQPlayerBase::EndSwordAttack()
{
	if (mbIsPressedX)
	{
		mbIsPressedX = false;
	}
}

void AFQPlayerBase::StartDash()
{
	if (!mbIsDashing)
	{
		mbCanDash = false;
		mbIsDashing = true;
		mDashElapsedTime = 0.0f;

		GetCharacterMovement()->MaxWalkSpeed = mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = 200000.0f;

		GetWorld()->GetTimerManager().SetTimer(mDashCoolTimer, this, &AFQPlayerBase::ResetDash, mDashCoolTime, false);
	}
}

void AFQPlayerBase::EndDash()
{
	mbIsDashing = false;

	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void AFQPlayerBase::ResetDash()
{
	mbCanDash = true;
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

void AFQPlayerBase::ResetCombo()
{
	UE_LOG(LogTemp, Log, TEXT("[Timer] ResetCombo"));

	mSwordAttackComboState = EKnightSwordAttackComboState::None;

	mSwordAttackState = EKnightSwordAttackState::CoolDown;

	GetWorld()->GetTimerManager().ClearTimer(mKnightCoolTimer);
	GetWorld()->GetTimerManager().SetTimer(mKnightCoolTimer, this, &AFQPlayerBase::ResetCoolDown, mSwordAttackCoolTime, false);
}

void AFQPlayerBase::ResetCoolDown()
{
	UE_LOG(LogTemp, Log, TEXT("[Timer] ResetCoolDown"));

	mSwordAttackState = EKnightSwordAttackState::None;
}

void AFQPlayerBase::ProcessSwordAttack()
{
	if (mbIsPressedX)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	switch (mSwordAttackState)
	{
	case EKnightSwordAttackState::Attack1:
	{
		UE_LOG(LogTemp, Log, TEXT("[ProcessSwordAttack] SwordAttack1"));

		if (!AnimInstance->Montage_IsPlaying(mSwordAttackAnim))
		{
			AnimInstance->Montage_Play(mSwordAttackAnim);
;
			mSwordAttackComboState = EKnightSwordAttackComboState::CanBeCombo;

			GetWorld()->GetTimerManager().ClearTimer(mKnightComboTimer);

			GetWorld()->GetTimerManager().SetTimer(mKnightComboTimer, this, &AFQPlayerBase::ResetCombo, mSwordAttackWaitTime1, false);
		}
	}
	break;
	case EKnightSwordAttackState::Attack2:
	{
		if (mSwordAttackComboState != EKnightSwordAttackComboState::Combo)
		{
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("[ProcessSwordAttack] SwordAttack2"));

		if (!AnimInstance->Montage_IsPlaying(mSwordAttackAnim))
		{
			AnimInstance->Montage_Play(mSwordAttackAnim);
		}

		AnimInstance->Montage_JumpToSection(FName("SwordAttack2"), mSwordAttackAnim);

		mSwordAttackComboState = EKnightSwordAttackComboState::CanBeCombo;

		GetWorld()->GetTimerManager().ClearTimer(mKnightComboTimer);

		GetWorld()->GetTimerManager().SetTimer(mKnightComboTimer, this, &AFQPlayerBase::ResetCombo, mSwordAttackWaitTime2, false);
	}
	break;
	case EKnightSwordAttackState::Attack3:
	{
		if (mSwordAttackComboState != EKnightSwordAttackComboState::Combo)
		{
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("[ProcessSwordAttack] SwordAttack3"));

		if (!AnimInstance->Montage_IsPlaying(mSwordAttackAnim))
		{
			AnimInstance->Montage_Play(mSwordAttackAnim);
		}

		AnimInstance->Montage_JumpToSection(FName("SwordAttack3"), mSwordAttackAnim);

		GetWorld()->GetTimerManager().ClearTimer(mKnightComboTimer);

		ResetCombo();
	}
	break;
	}
}

void AFQPlayerBase::ProcessNextSection(const FName& SectionName)
{
	if (!mbIsPressedX)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (!AnimInstance->Montage_IsPlaying(mSwordAttackAnim))
	{
		UE_LOG(LogTemp, Log, TEXT("[ProcessNextSection] Section Name : %s"), *SectionName.ToString());

		AnimInstance->Montage_Play(mSwordAttackAnim);
		AnimInstance->Montage_JumpToSection(SectionName, mSwordAttackAnim);
	}

	if (SectionName == FName("SwordAttack2"))
	{
		mSwordAttackState = EKnightSwordAttackState::Attack2;
	}
	else if (SectionName == FName("SwordAttack3"))
	{
		mSwordAttackState = EKnightSwordAttackState::Attack3;

		GetWorld()->GetTimerManager().ClearTimer(mKnightComboTimer);

		ResetCombo();
	}
}

void AFQPlayerBase::PressedSwordAttack()
{
	mbIsPressedX = true;
}
