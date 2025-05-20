// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"

#include "Blueprint/UserWidget.h"

#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQGameMode_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul\public\FQSoulBase.h"
#include "FQPlayer\Public\FQPlayerBase.h"
#include "FQUI/Player/FQPlayerHUDWidget.h"

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

void AFQPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();

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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild AFQGameMode_InGame"), __LINE__);
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
