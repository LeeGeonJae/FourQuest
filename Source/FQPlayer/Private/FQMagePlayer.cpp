// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMagePlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

#include "FQPlayer/Public/FQMageDataAsset.h" 
#include "FQPlayer/Public/FQMageProjectile.h" 
#include "FQGameCore/Player/FQPlayerAttackableInterface.h"

AFQMagePlayer::AFQMagePlayer()
{
	mProjectileAttackVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileAttackVolume"));
	mProjectileAttackVolume->SetupAttachment(RootComponent);

	mLookAtRotation = FRotator::ZeroRotator;
}

void AFQMagePlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!mMageDataAsset)
	{
		return;
	}

	if (mProjectileAttackState != EMageProjectileAttackState::None && mProjectileAttackState != EMageProjectileAttackState::CoolDown)
	{
		SetActorRotation(mLookAtRotation);
	}
}

void AFQMagePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Projectile Attack (X)
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Started, this, &AFQMagePlayer::StartProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Triggered, this, &AFQMagePlayer::PressedProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Canceled, this, &AFQMagePlayer::EndProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Completed, this, &AFQMagePlayer::EndProjectileAttack);
}

void AFQMagePlayer::ProcessNextSection()
{
	if (!mbIsPressedX)
	{
		return;
	}

	if (mProjectileAttackComboState == EComboState::CanBeCombo)
	{
		mProjectileAttackComboState = EComboState::Combo;
	}
}

void AFQMagePlayer::EnableAttackVolume()
{
	if (!mProjectileAttackVolume)
	{
		return;
	}

	mProjectileAttackVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CheckProjectileAttackVolume();
}

void AFQMagePlayer::DisableAttackVolume()
{
	if (!mProjectileAttackVolume)
	{
		return;
	}

	mProjectileAttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQMagePlayer::BeginPlay()
{
	Super::BeginPlay();

	// Mage IMC 바인딩
	SetInputMappingContext();

	// Mage 에셋에서 기본 속도 설정
	if (mMageDataAsset)
	{
		mDefaultSpeed = mMageDataAsset->mDefaultSpeed;
		GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	}

	// Animation MontageEnded 바인딩
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}
	AnimInstance->OnMontageEnded.AddDynamic(this, &AFQMagePlayer::OnAnimMontageEnded);
}

void AFQMagePlayer::SetInputMappingContext()
{
	Super::SetInputMappingContext();

	if (!mMageMappingContext)
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mMageMappingContext, 1);
	}
}

bool AFQMagePlayer::CanMove()
{
	// TODO : 이동 제한 조건 설정
	switch (mProjectileAttackState)
	{
	case EMageProjectileAttackState::Attack1:
	{
		if (mbIsPressedX)
		{
			return false;
		}
		else
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return false;
			}

			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
			{
				return false;
			}
		}
	}
	break;
	case EMageProjectileAttackState::Attack2:
	{
		if (mbIsPressedX)
		{
			return false;
		}
		else
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return false;
			}

			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
			{
				return false;
			}
		}
	}
	break;
	}

	return true;
}

void AFQMagePlayer::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == mProjectileAttackAnim1)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mProjectileAttackComboState == EComboState::Combo)
		{
			mProjectileAttackState = EMageProjectileAttackState::Attack2;
			mProjectileAttackComboState = EComboState::CanBeCombo;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
			{
				return;
			}

			AnimInstance->Montage_Play(mProjectileAttackAnim2);

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
		}
	}
	else if (Montage == mProjectileAttackAnim2)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mProjectileAttackComboState == EComboState::Combo)
		{
			mProjectileAttackState = EMageProjectileAttackState::Attack1;
			mProjectileAttackComboState = EComboState::CanBeCombo;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
			{
				return;
			}

			AnimInstance->Montage_Play(mProjectileAttackAnim1);

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime2, false);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			ResetProjectileAttackCombo();
		}
	}
}

void AFQMagePlayer::StartProjectileAttack()
{
	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	if (mProjectileAttackComboState == EComboState::None)
	{
		// 공격 입력이 처음으로 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartProjectileAttack] 공격 입력이 처음으로 들어왔을 때"));

		if (mProjectileAttackState == EMageProjectileAttackState::CoolDown)
		{
			return;
		}

		// 공격 상태 설정
		mProjectileAttackState = EMageProjectileAttackState::Attack1;

		// 애니메이션 재생
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}
		AnimInstance->Montage_Play(mProjectileAttackAnim1);

		// 콤보 가능 상태 설정
		mProjectileAttackComboState = EComboState::CanBeCombo;
		GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
		GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
	}
	else if (mProjectileAttackComboState == EComboState::CanBeCombo)
	{
		// 입력 대기 시간 내에 입력이 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartProjectileAttack] 입력 대기 시간 내에 입력이 들어왔을 때"));

		// 콤보 가능 상태 설정
		mProjectileAttackComboState = EComboState::Combo;

		switch (mProjectileAttackState)
		{
		case EMageProjectileAttackState::Attack1 :
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
			{
				return;
			}

			mProjectileAttackState = EMageProjectileAttackState::Attack2;

			AnimInstance->Montage_Play(mProjectileAttackAnim2);

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
		}
		break;
		case EMageProjectileAttackState::Attack2:
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
			{
				return;
			}

			mProjectileAttackState = EMageProjectileAttackState::Attack1;

			AnimInstance->Montage_Play(mProjectileAttackAnim1);
		}
		break;
		}
	}
}

void AFQMagePlayer::PressedProjectileAttack()
{
	mbIsPressedX = true;
}

void AFQMagePlayer::EndProjectileAttack()
{
	if (mbIsPressedX)
	{
		mbIsPressedX = false;
	}
}

void AFQMagePlayer::ResetProjectileAttackCombo()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetProjectileAttackCombo] ResetCombo"));

	mProjectileAttackComboState = EComboState::None;

	mProjectileAttackState = EMageProjectileAttackState::CoolDown;

	GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackCoolTimer);
	GetWorld()->GetTimerManager().SetTimer(mProjectileAttackCoolTimer, this, &AFQMagePlayer::ResetProjectileAttackCoolDown, mMageDataAsset->mProjectileAttackCoolTime, false);
}

void AFQMagePlayer::ResetProjectileAttackCoolDown()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetProjectileAttackCoolDown] ResetCoolDown"));

	mProjectileAttackState = EMageProjectileAttackState::None;
}

bool AFQMagePlayer::ApplyPush(AActor* AttackableActor)
{
	IFQPlayerAttackableInterface* PlayerAttackableInterface = Cast<IFQPlayerAttackableInterface>(AttackableActor);
	if (!PlayerAttackableInterface)
	{
		return false;
	}

	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.f;
	Forward.Normalize();

	PlayerAttackableInterface->TakePushByPlayer(AttackableActor, Forward, mMageDataAsset->mProjectileStrength);

	return true;
}

void AFQMagePlayer::ProcessProjectileAttack()
{
	if (!mProjectile)
	{
		return;
	}

	// 투사체 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	AFQMageProjectile* Projectile = GetWorld()->SpawnActor<AFQMageProjectile>(mProjectile, GetActorLocation(), GetActorRotation(), SpawnParams);
	
	if (Projectile)
	{
		// 투사체 초기화 및 발사
		Projectile->SetLifeSpan(mMageDataAsset->mProjectileDuration);
		Projectile->SetCount(mMageDataAsset->mProjectileCount);
		Projectile->LaunchProjectile(mLookAtDirection, mMageDataAsset->mProjectileSpeed);

		UE_LOG(LogTemp, Log, TEXT("[ProcessProjectileAttack] 생성 성공"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[ProcessProjectileAttack] 생성 실패"));
	}
}

void AFQMagePlayer::CheckProjectileAttackVolume()
{
	float Distance = TNumericLimits<float>::Max();
	TArray<AActor*> OverlappedActors;
	mProjectileAttackVolume->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		if (!Actor)
		{
			return;
		}

		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (!RootComp)
		{
			return;
		}

		if (RootComp == GetRootComponent())
		{
			continue;
		}

		if (mMageDataAsset->mProjectileAttackableTypes.IsEmpty())
		{
			return;
		}

		if (!mMageDataAsset->mProjectileAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
		{
			continue;
		}

		float NewDistance = FVector::Distance(GetActorLocation(), Actor->GetActorLocation());

		if (Distance <= NewDistance)
		{
			continue;
		}
		 
		Distance = NewDistance;
		mLookAtDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		mLookAtDirection.Z = 0.0f;
		mLookAtRotation = mLookAtDirection.Rotation();
	}
}
