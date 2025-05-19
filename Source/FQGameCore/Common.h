#pragma once

UENUM()
enum class EArmourType : uint8
{
	Knight,
	Magic,
	End
};

UENUM()
enum class ESoulType : uint8
{
	Knight,
	Magic,
	End
};

#pragma region FQPlayerState_InGame Delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulTypeChangeDelegate, ESoulType /*NewSoulType*/);		// PlayerState의 SoulType 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FQArmourTypeChangeDelegate, EArmourType /*NewArmourType*/);		// PlayerState의 ArmourType 변경 델리게이트
#pragma endregion
