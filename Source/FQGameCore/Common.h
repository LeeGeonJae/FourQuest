#pragma once

UENUM()
enum class EArmourType : uint8
{
	Knight = 0,
	Magic,
	End
};

UENUM()
enum class ESoulType : uint8
{
	Sword,	// 검 영혼 0
	Staff,	// 마법 영혼 1
	Axe,	// 도끼 영혼 2
	Bow,	// 활 영혼 3
	End
};

UENUM()
enum class EWidgetInputType : uint8
{
	None = 0,
	Left,
	Right,
	Up,
	Down,
	Select,
	Cancel,
	End
};

UENUM()
enum class ETitleSettingType : uint8
{
	Title = 0,
	MainSetting,
	VideoSetting,
	AudioSetting,
	End
};

namespace FQ_InGameSetting
{
	UENUM()
		enum class EResolutionType : uint8
	{
		Resolution_1080x600,
		Resolution_1280x720,
		Resolution_1600x900,
		Resolution_1920x1080,
	};

	struct FQLocalMultiPlayerInfomation
	{
		FQLocalMultiPlayerInfomation() : bSpawnLocalPlayer(false), mSoulType(ESoulType::End), mArmourType(EArmourType::End) {}

		uint8 bSpawnLocalPlayer : 1;
		ESoulType mSoulType;
		EArmourType mArmourType;
	};

	struct FAudioInfomation
	{
		FAudioInfomation() : mbIsFullVolumMute(false), mbIsBGMVolumMute(false), mbIsSFXVolumMute(false), mFullVolumValue(0.8f), mBGMVolumValue(0.8f), mSFXVolumValue(0.8f) {}

		uint8 mbIsFullVolumMute : 1;
		uint8 mbIsBGMVolumMute : 1;
		uint8 mbIsSFXVolumMute : 1;

		float mFullVolumValue;
		float mBGMVolumValue;
		float mSFXVolumValue;
	};

	struct FVideoInfomation
	{
		FVideoInfomation() : mbIsFullScreen(false), mCurrentResolutionType(EResolutionType::Resolution_1920x1080){}

		uint8 mbIsFullScreen : 1;
		EResolutionType mCurrentResolutionType;
	};
}

namespace FQ_PlayerStateDelegate
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulTypeChangeDelegate, ESoulType /*NewSoulType*/);					// PlayerState의 SoulType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQArmourTypeChangeDelegate, EArmourType /*NewArmourType*/);				// PlayerState의 ArmourType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulGaugeSettingDelegate, float /*GaugeValue*/);						// PlayerState의 SoulGauge 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQPlayerHpUpdateDelegate, float /*HpValue*/);							// PlayerState의 Hp 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQPlayerDeadDelegate, bool /*bIsDead*/);								// PlayerState의 Dead 호출 델리게이트
}

namespace FQ_UIDelegate
{
	DECLARE_DELEGATE_OneParam(FQTitleSettingDelegate, ETitleSettingType /*TitleSettingType*/);
}