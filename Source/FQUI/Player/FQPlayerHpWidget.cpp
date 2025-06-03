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
	TSoftObjectPtr<UTexture2D>* TexturePtr = nullptr;
	switch (PlayerControllerNumber)
	{
	case 0:
		TexturePtr = mPlayerMap1.Find(SoulType);
		break;
	case 1:
		TexturePtr = mPlayerMap2.Find(SoulType);
		break;
	case 2:
		TexturePtr = mPlayerMap3.Find(SoulType);
		break;
	case 3:
		TexturePtr = mPlayerMap4.Find(SoulType);
		break;
	}

	if (TexturePtr)
	{
		UTexture2D* LoadedTexture = TexturePtr->LoadSynchronous();
		if (LoadedTexture)
		{
			mPlayerNumber->SetBrushFromTexture(LoadedTexture);
			UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHpWidget %d] mPlayerNumber Image Changed Successfully"), __LINE__);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] Failed to Load Texture for SoulType: %d"), __LINE__, (int32)SoulType);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] TexturePtr is invalid or not found for SoulType: %d"), __LINE__, (int32)SoulType);
	}
}
