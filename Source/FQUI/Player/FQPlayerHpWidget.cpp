#include "FQPlayerHpWidget.h"
#include "Components/Image.h"

UFQPlayerHpWidget::UFQPlayerHpWidget()
	: mHpPercent(1.f)
	, mHpDecrasePercent(1.f)
	, mMaxSize()
{
}

void UFQPlayerHpWidget::NativeConstruct()
{
	if (!mHp)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHp Image Is Null!!"), __LINE__);
		return;
	}

	// 현재 Hp 이미지 가로 사이즈 값 초기화 적용
	const FSlateBrush& HpBrush = mHp->GetBrush();
	mMaxSize = HpBrush.ImageSize.X;
}

void UFQPlayerHpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!mHpDecrase)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHpDecrase Image Is Null!!"), __LINE__);
		return;
	}

	if (mHpDecrasePercent < mHpPercent)
	{
		FSlateBrush HpDecraseBrush = mHpDecrase->GetBrush();

		mHpDecrasePercent -= (InDeltaTime * 0.5f);
		HpDecraseBrush.ImageSize.X = mMaxSize * mHpDecrasePercent;
	}
}

void UFQPlayerHpWidget::UpdateHp(float HpValue)
{
	if (!mHp)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHp Image Is Null!!"), __LINE__);
		return;
	}

	mHpPercent = HpValue;

	FSlateBrush HpBrush = mHp->GetBrush();
	HpBrush.ImageSize.X = mMaxSize * mHpPercent;
}

void UFQPlayerHpWidget::UpdatePlayerControllerNumber(int32 PlayerControllerNumber, ESoulType SoulType)
{
	if (!mPlayerNumber)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mPlayerNumber Image Is Null!!"), __LINE__);
		return;
	}

	// 플레이어 넘버 세팅
	switch (PlayerControllerNumber)
	{
	case 0:
	{
		if (mPlayerMap1[SoulType] && mPlayerMap1[SoulType].IsValid())
		{
			UTexture2D* SettingTexture = mPlayerMap1[SoulType].LoadSynchronous();
			mPlayerNumber->SetBrushFromTexture(SettingTexture);
		}
	}
	break;
	case 1:
	{
		if (mPlayerMap2[SoulType] && mPlayerMap2[SoulType].IsValid())
		{
			UTexture2D* SettingTexture = mPlayerMap1[SoulType].LoadSynchronous();
			mPlayerNumber->SetBrushFromTexture(SettingTexture);
		}
	}
	break;
	case 2:
	{
		if (mPlayerMap3[SoulType] && mPlayerMap3[SoulType].IsValid())
		{
			UTexture2D* SettingTexture = mPlayerMap1[SoulType].LoadSynchronous();
			mPlayerNumber->SetBrushFromTexture(SettingTexture);
		}
	}
	break;
	case 3:
	{
		if (mPlayerMap4[SoulType] && mPlayerMap4[SoulType].IsValid())
		{
			UTexture2D* SettingTexture = mPlayerMap1[SoulType].LoadSynchronous();
			mPlayerNumber->SetBrushFromTexture(SettingTexture);
		}
	}
	break;
	}
}
