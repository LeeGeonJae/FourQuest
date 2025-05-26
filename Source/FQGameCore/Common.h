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
	Knight = 0,
	Magic,
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

namespace FQ_LocalMulti
{
	USTRUCT()
	struct FQLocalMultiPlayerInfomation
	{
		FQLocalMultiPlayerInfomation() : bSpawnLocalPlayer(false), mSoulType(ESoulType::End), mArmourType(EArmourType::End) {}

		uint8 bSpawnLocalPlayer : 1;
		ESoulType mSoulType;
		EArmourType mArmourType;
	};
}

namespace FQ_PlayerStateDelegate
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulTypeChangeDelegate, ESoulType /*NewSoulType*/);					// PlayerState의 SoulType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQArmourTypeChangeDelegate, EArmourType /*NewArmourType*/);				// PlayerState의 ArmourType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulGaugeSettingDelegate, float /*GaugeValue*/);						// PlayerState의 SoulGauge 변경 델리게이트
}