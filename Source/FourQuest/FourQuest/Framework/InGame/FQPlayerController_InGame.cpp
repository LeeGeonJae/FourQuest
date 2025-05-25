// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
#include "FQGameMode_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul\public\FQSoulBase.h"
#include "FQPlayer\Public\FQPlayerBase.h"
#include "FQUI/Player/FQPlayerHUDWidget.h"
#include "FQGameCore\Player\FQPlayerInputDataAsset.h"

AFQPlayerController_InGame::AFQPlayerController_InGame()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDWidget(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
	if (PlayerHUDWidget.Class)
	{
		mPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget.Class);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Faild Load Path : /Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget"), __LINE__);
	}
}

void AFQPlayerController_InGame::UpdateHUDSetting()
{
	// 플레이어 스테이트 클래스 확인
	AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (FQPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild FQPlayerState_InGame"), __LINE__);
		return;
	}

	// HUD Widget 함수 Binding
	FQPlayerState->mSoulChangeDelegate.AddLambda([&](ESoulType NewSoulType)
		{
			UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
			FQPlayerHUDWidget->UpdateSoulIcon(NewSoulType);
		});
	FQPlayerState->mArmourChangeDelegate.AddLambda([&](EArmourType NewArmourType)
		{
			UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
			FQPlayerHUDWidget->UpdateArmourSkill(NewArmourType);
		});
	FQPlayerState->mSoulGaugeDelegate.AddLambda([&](float CurrentSoulGauge)
		{
			UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
			FQPlayerHUDWidget->UpdateSoulGauge(CurrentSoulGauge);
		});

	// UFQPlayerHUDWidget에 소유 오브젝트 등록
	UFQPlayerHUDWidget* FQPlayerHUDWidget = Cast<UFQPlayerHUDWidget>(mPlayerHUDWidget);
	if (FQPlayerHUDWidget)
	{
		FQPlayerHUDWidget->SetOwningActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild UFQPlayerHUDWidget"), __LINE__);
	}

	// 게임 모드에 플레이어 컨트롤러 등록
	AFQGameMode_InGame* FQGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
	if (FQGameMode)
	{
		FQGameMode->GetPlayerHUDManager()->AddPlayerController(this, mPlayerHUDWidget);

		AFQMainCenterCamera* Camera = FQGameMode->GetMainCamera();
		if (Camera)
		{
			SetViewTargetWithBlend(Camera);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Not Find CameraActor!!"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild AFQGameMode_InGame!!"), __LINE__);
	}
}

void AFQPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();

	// Local Player Subsystem에 MappingContext 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mPlayerInputDataAsset->mDefaultMappingContext, 0);
	}
}



void AFQPlayerController_InGame::ChangeToArmour(EArmourType InArmourType)
{
	APawn* PlayerCharacter = GetPawn();
	FTransform SpawnTransform = FTransform(PlayerCharacter->GetActorTransform().GetLocation());
	if (PlayerCharacter)
	{
		PlayerCharacter->Destroy();
	}

	// 사용할 블루프린트 클래스 로딩
	TSubclassOf<AFQPlayerBase> CharacterBPClass = mPlayerArmourCharacterClasses[InArmourType];
	if (CharacterBPClass)
	{
		CreatePlayerCharacterByClass(CharacterBPClass, SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Failed to load character blueprint class"), __LINE__);
	}

	// 플레이어 스테이트 세팅
	AFQPlayerState_InGame* MyPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (MyPlayerState)
	{
		MyPlayerState->SetArmourType(InArmourType);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] AFQPlayerState_InGame Is Not Vaild!!"), __LINE__);
	}
}

void AFQPlayerController_InGame::ChangeToSoul()
{
	APawn* PlayerCharacter = GetPawn();
	FTransform SpawnTransform = FTransform(PlayerCharacter->GetActorTransform().GetLocation());
	if (PlayerCharacter)
	{
		PlayerCharacter->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Character Is nullptr"), __LINE__);
		return;
	}

	// 플레이어 스테이트 세팅
	AFQPlayerState_InGame* MyPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (MyPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] AFQPlayerState_InGame Is Not Vaild!!"), __LINE__);
		return;
	}

	MyPlayerState->SetArmourType(EArmourType::End);
	ESoulType PlayerSoulType = MyPlayerState->GetSoulType();

	// 사용할 블루프린트 클래스 로딩
	TSubclassOf<AFQSoulBase> CharacterBPClass = mPlayerSoulCharacterClasses[PlayerSoulType];
	if (CharacterBPClass)
	{
		CreateSoulCharacterByClass(CharacterBPClass, SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Failed to load character blueprint class"), __LINE__);
	}
}

void AFQPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	// EnhancedInputComponent 사용
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] InputComponent Is nullptr"), __LINE__);
		return;
	}

	if (mPlayerInputDataAsset)
	{
		EnhancedInput->BindAction(mPlayerInputDataAsset->mAButtonAction, ETriggerEvent::Started, this, &AFQPlayerController_InGame::HandlePickButton);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mBButtonAction, ETriggerEvent::Started, this, &AFQPlayerController_InGame::HandleCancelButton);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Triggered, this, &AFQPlayerController_InGame::HandleMoveTriggered);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Completed, this, &AFQPlayerController_InGame::HandleMoveCompleted);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] mPlayerInputDataAsset Is Not Vaild!!"), __LINE__);
	}
}



void AFQPlayerController_InGame::CreatePlayerCharacterByClass(TSubclassOf<AFQPlayerBase> CharacterClass, const FTransform& SpawnTransform)
{
	if (!CharacterClass) return;

	// 캐릭터 생성
	AFQPlayerBase* NewCharacter = GetWorld()->SpawnActorDeferred<AFQPlayerBase>(CharacterClass, SpawnTransform);
	if (NewCharacter)
	{
		Possess(NewCharacter);							// 플레이어 캐릭터 등록
		NewCharacter->FinishSpawning(SpawnTransform);	// 캐릭터 초기화 끝 BeginPlay 함수 실행

		// 카메라 등록
		AFQGameMode_InGame* InGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
		if (InGameMode)
		{
			SetViewTargetWithBlend(InGameMode->GetMainCamera());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame &d] AFQGameMode_InGame Is Not Vailed!!"), __LINE__);
		}
	}
}

void AFQPlayerController_InGame::CreateSoulCharacterByClass(TSubclassOf<class AFQSoulBase> CharacterClass, const FTransform& SpawnTransform)
{
	if (!CharacterClass) return;

	// 캐릭터 생성
	AFQSoulBase* NewCharacter = GetWorld()->SpawnActorDeferred<AFQSoulBase>(CharacterClass, SpawnTransform);
	if (NewCharacter)
	{
		NewCharacter->FinishSpawning(SpawnTransform);	// 캐릭터 초기화 끝 BeginPlay 함수 실행
		Possess(NewCharacter);							// 플레이어 캐릭터 등록

		// 카메라 등록
		AFQGameMode_InGame* InGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
		if (InGameMode)
		{
			SetViewTargetWithBlend(InGameMode->GetMainCamera());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame &d] AFQGameMode_InGame Is Not Vailed!!"), __LINE__);
		}
	}
}

void AFQPlayerController_InGame::HandlePickButton()
{
	IFQGameModeInterface* MyGameMode = Cast<IFQGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->SelectInteraction();
	}
}

void AFQPlayerController_InGame::HandleCancelButton()
{
	IFQGameModeInterface* MyGameMode = Cast<IFQGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->CancelInteraction();
	}
}

void AFQPlayerController_InGame::HandleMoveTriggered(const FInputActionValue& Value)
{
	// 방향 업데이트
	mMoveDirection = Value.Get<FVector2D>();

	if (mbIsMoveKeyHeld)
	{
		return;
	}
	if (FMath::Abs(mMoveDirection.X) >= 0.8f || FMath::Abs(mMoveDirection.Y) >= 0.8f)
	{
		mbIsMoveKeyHeld = true;
		DoMove();
	}

	// 반복 타이머 설정 (무조건 실행)
	GetWorldTimerManager().SetTimer(
		mRepeatMoveTimerHandle,
		this,
		&AFQPlayerController_InGame::DoMove,
		0.2f,
		true,
		1.0f
	);
}

void AFQPlayerController_InGame::HandleMoveCompleted(const FInputActionValue& Value)
{
	mbIsMoveKeyHeld = false;
	GetWorldTimerManager().ClearTimer(mRepeatMoveTimerHandle);
}

void AFQPlayerController_InGame::DoMove()
{
	if (FMath::Abs(mMoveDirection.X) < 0.8f && FMath::Abs(mMoveDirection.Y) < 0.8f)
	{
		return;
	}

	IFQGameModeInterface* MyGameMode = Cast<IFQGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->MoveButton(mMoveDirection);
	}
}
