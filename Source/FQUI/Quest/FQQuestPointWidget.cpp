#include "FQQuestPointWidget.h"

#include "FQGameCore\Quest\FQQuestSystem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"

UFQQuestPointWidget::UFQQuestPointWidget()
	: mQuestID()
	, mQuestSignType(EQuestSignType::Boss)
	, mbIsQuestActive(false)
{
}

void UFQQuestPointWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFQQuestPointWidget::UpdateQuestPosition(FVector2D NormalDirection)
{
	if (!GEngine || !GEngine->GameViewport || !mSignBox)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestPointWidget %d] GEngine Or GameViewport Or mSignBox가 유효하지 않습니다!! "), __LINE__);
		return;
	}

	// 1. 화면 크기 가져오기
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// 2. 방향 벡터 정규화 (안정성 보장)
	if (!NormalDirection.Normalize())
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestPointWidget %d] NormalDirection 정규화가 되지 않았습니다!!"), __LINE__);
		return;
	}

	// 3. 화면 중앙
	const FVector2D ScreenCenter = ViewportSize * 0.5f;

	// 4. 방향을 화면 반 사이즈까지 확장
	FVector2D ScaledDirection = NormalDirection * ScreenCenter;

	// 5. 화면 경계 넘지 않도록 클리핑 (비율로 줄이기)
	float XRatio = ScreenCenter.X / FMath::Max(FMath::Abs(ScaledDirection.X), 1.0f);
	float YRatio = ScreenCenter.Y / FMath::Max(FMath::Abs(ScaledDirection.Y), 1.0f);
	float FinalRatio = FMath::Min(XRatio, YRatio);
	ScaledDirection *= FinalRatio;

	// 6. 최종 위치 = 중앙 + 스케일된 방향
	FVector2D FinalScreenPosition = ScreenCenter + ScaledDirection;

	// 7. VerticalBox의 Slot이 CanvasPanelSlot이면 위치 설정
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSignBox->Slot))
	{
		// 필요 시 Anchor 또는 Alignment도 조정 가능
		CanvasSlot->SetPosition(FinalScreenPosition);
	}
}

void UFQQuestPointWidget::UpdateQuestActive(bool bIsActive)
{
	if (bIsActive)
	{
		mSign->SetBrushFromTexture(mQuestActiveSignTexture[mQuestSignType]);
	}
	else
	{
		mSign->SetBrushFromTexture(mQuestInactiveSignTexture[mQuestSignType]);
	}
}
