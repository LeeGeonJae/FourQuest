
#include "FQPlayerUIActor.h"
#include "FQUI\FQWidgetComponent.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerState_InGame.h"
#include "FQUI\Player\FQPlayerHpWidget.h"
#include "FQUI\Soul\FQSoulGaugeWidget.h"

AFQPlayerUIActor::AFQPlayerUIActor()
{
	mPlayerHpWidgetComponent = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("PlayerHpWidget"));
	mArmourGaugeWidgetComponent = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("GaugeWidget"));
	mPlayerHpWidgetComponent->SetupAttachment(RootComponent);
	mArmourGaugeWidgetComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UFQPlayerHpWidget> PlayerHpWidgetRef(TEXT("/Game/Blueprints/Player/WBP_PlayerHp.WBP_PlayerHp_C"));
	if (PlayerHpWidgetRef.Class)
	{
		mPlayerHpWidgetComponent->SetWidgetClass(PlayerHpWidgetRef.Class);
		mPlayerHpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mPlayerHpWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
		mPlayerHpWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}

	static ConstructorHelpers::FClassFinder<UFQSoulGaugeWidget> GaugeWidgetRef(TEXT("/Game/Blueprints/Player/WBP_ArmourGauge.WBP_ArmourGauge_C"));
	if (GaugeWidgetRef.Class)
	{
		mArmourGaugeWidgetComponent->SetWidgetClass(GaugeWidgetRef.Class);
		mArmourGaugeWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mArmourGaugeWidgetComponent->SetDrawSize(FVector2D(10.f, 10.f));
		mArmourGaugeWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mArmourGaugeWidgetComponent->SetVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mGaugeWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}
}

void AFQPlayerUIActor::BeginPlay()
{
	Super::BeginPlay();

	mPlayerHpWidget = Cast<UFQPlayerHpWidget>(mPlayerHpWidgetComponent->GetUserWidgetObject());
	if (!mPlayerHpWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
	}

	mArmourGaugeWidget = Cast<UFQSoulGaugeWidget>(mArmourGaugeWidgetComponent->GetUserWidgetObject());
	if (!mArmourGaugeWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
	}
}

void AFQPlayerUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFQPlayerUIActor::UpdatePlayerNumber(int32 PlayerControllerNumber, ESoulType PlayerSoulType)
{
	UE_LOG(LogTemp, Log, TEXT("[AFQPlayerUIActor %d] UpdatePlayerNumber 함수 실행 : (플레이어 컨트롤러 숫자 : %d, 플레이어 영혼 타입 : %d)"), __LINE__, PlayerControllerNumber, (int32)PlayerSoulType);
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
