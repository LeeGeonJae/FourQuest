// Copyright (Your Project)

#include "FQSoulBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "Blueprint\UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

#include "FQSoulDataAsset.h"
#include "FQGameCore/Armour/FQArmourInterface.h"
#include "FQGameCore\Player\FQPlayerControllerInterface.h"
#include "FQGameCore\Player\FQPlayerStateInterface.h"
#include "FQGameCore\Player\FQPlayerInputDataAsset.h"
#include "FQUI\FQWidgetComponent.h"
#include "FQUI/Soul/FQSoulGaugeWidget.h"
#include "Kismet\GameplayStatics.h"

AFQSoulBase::AFQSoulBase()
{
	// Movement State
	mDashDirection = FVector::ZeroVector;
	mbIsDashing = false;

	// Armour State
	mbIsPressedArmourChange = false;
	mArmourChangeTimer = 0.f;

	// Capsule Setup
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapEnd);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SoulCollision"));

	// Movement Setup
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// Widget Component
	mArmourGaugeWidget = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("Widget"));
	mArmourGaugeWidget->SetupAttachment(RootComponent);
	mArmourGaugeWidget->SetRelativeLocation(FVector(30.f, 0.f, 80.f));
	
	static ConstructorHelpers::FClassFinder<UFQSoulGaugeWidget> ArmourWidgetRef(TEXT("/Game/Blueprints/Soul/WBP_SoulArmourGaugeWidget.WBP_SoulArmourGaugeWidget_C"));
	if (ArmourWidgetRef.Class)
	{
		mArmourGaugeWidget->SetWidgetClass(ArmourWidgetRef.Class);
		mArmourGaugeWidget->SetWidgetSpace(EWidgetSpace::Screen);
		mArmourGaugeWidget->SetDrawSize(FVector2D(20.f, 20.f));
		mArmourGaugeWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mArmourGaugeWidget->SetVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] Is Not Vaild FQSoulGaugeWidget!!"), __LINE__);
	}
}

void AFQSoulBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;
}

void AFQSoulBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Dash Logic
	mDashCoolTimer -= DeltaTime;
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

	// Armour Logic
	CheckArmour(DeltaTime);
}

void AFQSoulBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(mPlayerInputDataAsset->mDefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] UEnhancedInputLocalPlayerSubsystem Is Not Vaild!!"), __LINE__);
		}
	}

	if (mPlayerInputDataAsset)
	{
		UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		Input->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Triggered, this, &AFQSoulBase::Move);
		Input->BindAction(mPlayerInputDataAsset->mBButtonAction, ETriggerEvent::Started, this, &AFQSoulBase::SelectInteraction);
		Input->BindAction(mPlayerInputDataAsset->mBButtonAction, ETriggerEvent::Completed, this, &AFQSoulBase::CancelInteraction);
		Input->BindAction(mPlayerInputDataAsset->mYButtonAction, ETriggerEvent::Triggered, this, &AFQSoulBase::AddSoulGauge);
		Input->BindAction(mPlayerInputDataAsset->mAButtonAction, ETriggerEvent::Triggered, this, &AFQSoulBase::StartDash);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SetupPlayerInputComponent %d] mPlayerInputDataAsset Is Not Vaild!!"), __LINE__);
	}
}

void AFQSoulBase::Move(const FInputActionValue& Value)
{
	if (mbIsDashing) return;

	FVector2D InputVec = Value.Get<FVector2D>();
	float SizeSq = InputVec.SquaredLength();
	float MoveScale = SizeSq > 1.0f ? 1.0f : FMath::Sqrt(SizeSq);

	if (SizeSq > 1.0f) InputVec.Normalize();

	FVector MoveDir = FVector(InputVec.X, InputVec.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDir).Rotator());
	AddMovementInput(MoveDir, MoveScale);
}

void AFQSoulBase::CheckArmour(float DeltaTime)
{
	IFQArmourInterface* Nearest = CheckNearArmour();
	IFQArmourInterface* CurrentArmourInterface = Cast<IFQArmourInterface>(mCurrentArmour);

	// 가장 가까운 갑옷이 다르면 취소
	if (CurrentArmourInterface != Nearest)
	{
		if (CurrentArmourInterface) CurrentArmourInterface->SetNearestArmour(false);
		if (Nearest) Nearest->SetNearestArmour(true);
		mCurrentArmour = Cast<UObject>(Nearest);
		CancelInteraction();
		return;
	}

	// 갑옷이 0개이거나 버튼을 안눌렀으면 중지
	if (!mbIsPressedArmourChange || mArmours.Num() == 0 || !mCurrentArmour) return;

	// 시간이 안되면 리턴 & UI 표시
	mArmourChangeTimer -= DeltaTime;
	if (mArmourChangeTimer > 0.f)
	{
		if (mArmourGaugeWidget)
		{
			FVector WorldOffset = GetActorLocation() + FVector(0.f, 30.f, 80.f); // 앞쪽 + 위쪽
			mArmourGaugeWidget->SetWorldLocation(WorldOffset);
		}

		UFQSoulGaugeWidget* GaugeWidget = Cast<UFQSoulGaugeWidget>(mArmourGaugeWidget->GetWidget());
		if (GaugeWidget)
		{
			GaugeWidget->SetChargeGaugeValueSet(mArmourChangeTimer / mSoulDataAsset->mArmourDelayTime);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d]GaugeWidget is nullptr!!"), __LINE__);
		}

		return;
	}

	// 갑옷 타입 확인한 후 갑옷 입기
	EArmourType Type = CurrentArmourInterface->GetArmourType();
	UE_LOG(LogTemp, Log, TEXT("Soul Pick Armour : %s"), *UEnum::GetValueAsString(Type));

	IFQPlayerControllerInterface* PlayerController = Cast<IFQPlayerControllerInterface>(GetController());
	if (PlayerController)
	{
		PlayerController->ChangeToArmour(Type);
	}

	CurrentArmourInterface->PickArmour();
}

void AFQSoulBase::StartDash()
{
	if (mSoulDataAsset && !mbIsDashing && mDashCoolTimer <= 0)
	{
		mbIsDashing = true;
		mDashTimer = mSoulDataAsset->mDashDuration;
		GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = mSoulDataAsset->mDashSpeed * 2;

		mDashDirection = GetLastMovementInputVector().IsZero() ? GetActorForwardVector() : GetLastMovementInputVector().GetSafeNormal();
	}
}

void AFQSoulBase::SelectInteraction()
{
	if (mCurrentArmour)
	{
		mArmourGaugeWidget->SetVisibility(true);
	}
	mbIsPressedArmourChange = true;
	mArmourChangeTimer = mSoulDataAsset->mArmourDelayTime;
}

void AFQSoulBase::CancelInteraction()
{
	mArmourGaugeWidget->SetVisibility(false);
	mbIsPressedArmourChange = false;
}

void AFQSoulBase::AddSoulGauge()
{
	IFQPlayerStateInterface* PlayerStateInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PlayerStateInterface)
	{
		PlayerStateInterface->AddSoulGauge(5);
	}
}

IFQArmourInterface* AFQSoulBase::CheckNearArmour()
{
	IFQArmourInterface* Closest = nullptr;
	float MinDistSq = FLT_MAX;
	FVector Origin = GetActorLocation();

	for (const auto& Entry : mArmours)
	{
		IFQArmourInterface* Armour = Cast<IFQArmourInterface>(Entry.Value);
		if (!Armour) continue;

		float DistSq = FVector::DistSquared(Origin, Armour->GetActorTransform().GetLocation());
		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			Closest = Armour;
		}
	}

	return Closest;
}


// Overlap
void AFQSoulBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (IFQArmourInterface* Armour = Cast<IFQArmourInterface>(OtherActor))
	{
		mArmours.Add(OtherActor->GetName(), OtherActor);
	}
}

void AFQSoulBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IFQArmourInterface* Armour = Cast<IFQArmourInterface>(OtherActor))
	{
		Armour->SetNearestArmour(false);
		mArmours.Remove(OtherActor->GetName());
	}
}


// IFQSoulCharacterInterface
FTransform AFQSoulBase::GetActorTransform() const
{
	return GetTransform();
}