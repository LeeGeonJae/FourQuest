
#include "FQPlayerUIActor.h"
#include "FQUI\FQWidgetComponent.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerState_InGame.h"
#include "FQUI\Player\FQPlayerHpWidget.h"
#include "FQUI\Soul\FQSoulGaugeWidget.h"

AFQPlayerUIActor::AFQPlayerUIActor()
{
	static ConstructorHelpers::FClassFinder<UFQPlayerHpWidget> PlayerHpWidgetRef(TEXT("/Game/Blueprints/Player/WBP_PlayerHp.WBP_PlayerHp_C"));
	if (PlayerHpWidgetRef.Class)
	{
		mPlayerHpWidgetComponent->SetWidgetClass(PlayerHpWidgetRef.Class);
		mPlayerHpWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
		mPlayerHpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mPlayerHpWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
		mPlayerHpWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mPlayerHpWidget = Cast<UFQPlayerHpWidget>(mPlayerHpWidgetComponent->GetUserWidgetObject());
		if (!mPlayerHpWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}

	static ConstructorHelpers::FClassFinder<UFQSoulGaugeWidget> GaugeWidgetRef(TEXT("/Game/Blueprints/Player/WBP_ArmourGauge.WBP_ArmourGauge_C"));
	if (GaugeWidgetRef.Class)
	{
		mArmourGaugeWidgetComponent->SetWidgetClass(GaugeWidgetRef.Class);
		mArmourGaugeWidgetComponent->SetRelativeLocation(FVector(40.f, 0.f, 100.f));
		mArmourGaugeWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mArmourGaugeWidgetComponent->SetDrawSize(FVector2D(10.f, 10.f));
		mArmourGaugeWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mArmourGaugeWidgetComponent->SetVisibility(false);

		mArmourGaugeWidget = Cast<UFQSoulGaugeWidget>(mArmourGaugeWidgetComponent->GetUserWidgetObject());
		if (!mArmourGaugeWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mGaugeWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}
}

void AFQPlayerUIActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFQPlayerUIActor::UpdatePlayerNumber(int32 PlayerControllerNumber, ESoulType PlayerSoulType)
{
	if (mPlayerHpWidget)
	{
		mPlayerHpWidget->UpdatePlayerControllerNumber(PlayerControllerNumber, PlayerSoulType);
	}
}

void AFQPlayerUIActor::UpdatePlayerHp(float HpValue)
{
	if (mPlayerHpWidget)
	{
		mPlayerHpWidget->UpdateHp(HpValue);
	}
}

void AFQPlayerUIActor::UpdateArmourGauge(bool bIsVisible, float GaugeValue)
{
	if (mArmourGaugeWidgetComponent && mArmourGaugeWidget)
	{
		mArmourGaugeWidgetComponent->SetVisibility(bIsVisible);
		mArmourGaugeWidget->UpdateChargeGaugeValue(GaugeValue);
	}
}
