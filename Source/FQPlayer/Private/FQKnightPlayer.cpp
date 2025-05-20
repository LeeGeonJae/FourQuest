// Fill out your copyright notice in the Description page of Project Settings.


#include "FQKnightPlayer.h"

#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

AFQKnightPlayer::AFQKnightPlayer()
{
	// Dash 
	mDashSpeed = 500.0f;
	mDashDuration = 0.5f;
	mDashCoolTime = 1.0f;
	mbCanDash = false;
	mbIsDashing = false;

	// Knight
	mSwordAttackState = EKnightSwordAttackState::None;
	mSwordAttackComboState = EKnightSwordAttackComboState::None;
	mSwordAttackCoolTime = 0.0f;
	mSwordAttackWaitTime1 = 0.0f;
	mSwordAttackWaitTime2 = 0.0f;

	mSwordAttackVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordAttackVolume"));
	mSwordAttackVolume->SetupAttachment(RootComponent);
}

void AFQKnightPlayer::Tick(float DeltaSeconds)
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

void AFQKnightPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(mDashAction, ETriggerEvent::Started, this, &AFQKnightPlayer::Dash);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Started, this, &AFQKnightPlayer::StartSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Triggered, this, &AFQKnightPlayer::PressedSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Completed, this, &AFQKnightPlayer::EndSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Canceled, this, &AFQKnightPlayer::EndSwordAttack);
}

void AFQKnightPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetInputMappingContext();
}

void AFQKnightPlayer::SetInputMappingContext()
{
	Super::SetInputMappingContext();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mKnightMappingContext, 1);
	}
}

void AFQKnightPlayer::ProcessNextSection(const FName& SectionName)
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

void AFQKnightPlayer::EnableSwordAttackVolume()
{
	if (!mSwordAttackVolume)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[EnableSwordAttackVolume]"));

	mSwordAttackVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	DrawDebugBox(GetWorld(), mSwordAttackVolume->GetComponentLocation(), mSwordAttackVolume->Bounds.BoxExtent, FColor::Magenta, false, 1.0f);
}

void AFQKnightPlayer::DisableSwordAttackVolume()
{
	if (!mSwordAttackVolume)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[DisableSwordAttackVolume]"));

	FlushPersistentDebugLines(GetWorld());
}

void AFQKnightPlayer::Dash()
{
	mbCanDash = true;

	if (mbCanDash)
	{
		StartDash();
	}
}

void AFQKnightPlayer::StartSwordAttack()
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

void AFQKnightPlayer::EndSwordAttack()
{
	if (mbIsPressedX)
	{
		mbIsPressedX = false;
	}
}

void AFQKnightPlayer::StartDash()
{
	if (!mbIsDashing)
	{
		mbCanDash = false;
		mbIsDashing = true;
		mDashElapsedTime = 0.0f;

		GetCharacterMovement()->MaxWalkSpeed = mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = 200000.0f;

		GetWorld()->GetTimerManager().SetTimer(mDashCoolTimer, this, &AFQKnightPlayer::ResetDash, mDashCoolTime, false);
	}
}

void AFQKnightPlayer::EndDash()
{
	mbIsDashing = false;

	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void AFQKnightPlayer::ResetDash()
{
	mbCanDash = true;
}

void AFQKnightPlayer::ResetCombo()
{
	UE_LOG(LogTemp, Log, TEXT("[Timer] ResetCombo"));

	mSwordAttackComboState = EKnightSwordAttackComboState::None;

	mSwordAttackState = EKnightSwordAttackState::CoolDown;

	GetWorld()->GetTimerManager().ClearTimer(mKnightCoolTimer);
	GetWorld()->GetTimerManager().SetTimer(mKnightCoolTimer, this, &AFQKnightPlayer::ResetCoolDown, mSwordAttackCoolTime, false);
}

void AFQKnightPlayer::ResetCoolDown()
{
	UE_LOG(LogTemp, Log, TEXT("[Timer] ResetCoolDown"));

	mSwordAttackState = EKnightSwordAttackState::None;
}

void AFQKnightPlayer::ProcessSwordAttack()
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

			GetWorld()->GetTimerManager().SetTimer(mKnightComboTimer, this, &AFQKnightPlayer::ResetCombo, mSwordAttackWaitTime1, false);
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

		GetWorld()->GetTimerManager().SetTimer(mKnightComboTimer, this, &AFQKnightPlayer::ResetCombo, mSwordAttackWaitTime2, false);
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

void AFQKnightPlayer::PressedSwordAttack()
{
	mbIsPressedX = true;
}
