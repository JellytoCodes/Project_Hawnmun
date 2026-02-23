// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunGameplayTags.h"



namespace HawnmunGameplayTags
{
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	void InitializeNativeGameplayTags()
	{
		// 데미지 타입별 애트리뷰트 속성
		DamageTypesToResistances.Add(Damage_Fire, Attributes_Resistance_Fire);
		DamageTypesToResistances.Add(Damage_Physical, Attributes_Resistance_Physical);

		// 데미지 타입별 발동 디버프
		DamageTypesToDebuffs.Add(Damage_Fire, Debuff_Fire);
		DamageTypesToDebuffs.Add(Damage_Physical, Debuff_Stun);
	}

#pragma region Input Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_Locked, "Input.Key.Locked")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_SpaceBar, "Input.Key.SpaceBar")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_LeftShift, "Input.Key.LeftShift")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_LeftCtrl, "Input.Key.LeftCtrl")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_LButton, "Input.Key.LButton")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_RButton, "Input.Key.RButton")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_MButton, "Input.Key.MButton")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_WheelUp, "Input.Key.WheelUp")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_WheelDown, "Input.Key.WheelDown")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_1, "Input.Key.1")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_2, "Input.Key.2")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Input_Key_3, "Input.Key.3")
#pragma endregion

#pragma region Cost Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cost_Sprint, "Cost.Sprint")
#pragma endregion

#pragma region Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_Rolling, "Ability.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_Sprint, "Ability.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_TargetLock, "Ability.TargetLock")
#pragma endregion

#pragma region State Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Stamina_RegenBlocked, "State.Stamina.RegenBlocked")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Stamina_Full, "State.Stamina.Full")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Dead, "State.Dead")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Rolling, "State.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_TargetLock, "State.TargetLock")
#pragma endregion

#pragma region Event Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_HitReact, "Event.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Invincible, "Event.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_SwitchTarget_Left, "Event.SwitchTarget.Left")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_SwitchTarget_Right, "Event.SwitchTarget.Right")
#pragma endregion

#pragma region DamageEffect Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Damage_Fire, "Damage.Fire")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Damage_Physical, "Damage.Physical")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Fire, "Debuff.Fire")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Stun, "Debuff.Stun")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Chance, "Debuff.Chance")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Damage, "Debuff.Damage")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Frequency, "Debuff.Frequency")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Debuff_Duration, "Debuff.Duration")
#pragma endregion

#pragma region Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Strafing, "Enemy.State.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_UnderAttack, "Enemy.State.UnderAttack")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Unblockable, "Enemy.State.Unblockable")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_Pig_MeleeAttack1, "Enemy.Pig.MeleeAttack1")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_Pig_MeleeAttack2, "Enemy.Pig.MeleeAttack2")
#pragma endregion

#pragma region Attribute Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Primary_Strength, "Attributes.Primary.Strength")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Primary_Resilience, "Attributes.Primary.Resilience")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Primary_Vigor, "Attributes.Primary.Vigor")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_Armor, "Attributes.Secondary.Armor")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_StaminaRegeneration, "Attributes.Secondary.StaminaRegeneration")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Secondary_MaxStamina, "Attributes.Secondary.MaxStamina")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Resistance_Fire, "Attributes.Resistance.Fire")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Resistance_Physical, "Attributes.Resistance.Physical")
#pragma endregion
}
