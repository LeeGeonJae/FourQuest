#pragma once

UENUM()
enum class EKnightSwordAttackState : uint8
{
	None,
	Attack1,
	Attack2,
	Attack3,
	CoolDown
};

UENUM()
enum class EKnightSwordAttackComboState : uint8
{
	None,
	CanBeCombo,			// 입력 대기 시간 내에 입력이 들어온다면, 콤보 가능 상태
	Combo				// 실제로 입력이 들어와 콤보가 된 상태
};

UENUM()
enum class EHitState : uint8
{
	None,
	HitReacting
};