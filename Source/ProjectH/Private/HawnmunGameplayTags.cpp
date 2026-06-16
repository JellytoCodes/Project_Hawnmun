// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunGameplayTags.h"

namespace HawnmunGameplayTags
{
	TArray<FGameplayTag> DamageTypes;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	void InitializeNativeGameplayTags()
	{
		// 데미지 타입별 발동 디버프
		DamageTypesToDebuffs.Add(Damage_Fire, Debuff_Fire);
		DamageTypesToDebuffs.Add(Damage_Physical, Debuff_Stun);

		DamageTypes.Add(Damage_Physical);
		DamageTypes.Add(Damage_Fire);
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

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Montage_FacingTarget, "Event.Montage.FacingTarget")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Montage_TakeOff, "Event.Montage.TakeOff")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Montage_SlamDown, "Event.Montage.SlamDown")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_WildBoar_Montage_LeapSlam, "Event.WildBoar.Montage.LeapSlam")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_WildBoar_Montage_DoubleSlam, "Event.WildBoar.Montage.DoubleSlam")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_WildBoar_Montage_FootStomp, "Event.WildBoar.Montage.FootStomp")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_WildBoar_Montage_Charge, "Event.WildBoar.Montage.Charge")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_IllusionEnd, "Event.NineTailed.IllusionEnd")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_FoxFireVolley, "Event.NineTailed.Montage.FoxFireVolley")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_EnhancedFoxFireVolley, "Event.NineTailed.Montage.EnhancedFoxFireVolley")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_FoxFireDropActor, "Event.NineTailed.Montage.FoxFireDropActor")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_FoxFireRandDrop, "Event.NineTailed.Montage.FoxFireRandDrop")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_PortalBite, "Event.NineTailed.Montage.PortalBite")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_PortalBreath, "Event.NineTailed.Montage.PortalBreath")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_NineTailed_Montage_EnhancedCharmIllusion, "Event.NineTailed.Montage.EnhancedCharmIllusion")
	
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Combat_SkillEnd, "Event.Combat.SkillEnd")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Equip_HasMainWeapon, "Event.Equip.HasMainWeapon")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Player_Montage_Fire, "Event.Player.Montage.Fire")
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

#pragma region Player Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Player_Attack_NormalAttack, "Player.Attack.NormalAttack")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Player_Attack_HaevyAttack, "Player.Attack.HeavyAttack")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Player_Attack_RangeAttack, "Player.Attack.RangeAttack")
#pragma endregion

#pragma region Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Boss_Phase2, "Enemy.State.Boss.Phase2")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Strafing, "Enemy.State.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Unblockable, "Enemy.State.Unblockable")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_Pig_MeleeAttack, "Enemy.Pig.MeleeAttack")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_WildBoar_LeftRightSwing, "Enemy.WildBoar.LeftRightSwing")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_WildBoar_DoubleSlam, "Enemy.WildBoar.DoubleSlam")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_WildBoar_FootStomp, "Enemy.WildBoar.FootStomp")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_WildBoar_LeapSlam, "Enemy.WildBoar.LeapSlam")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_WildBoar_Charge, "Enemy.WildBoar.Charge")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_FoxFireVolley, "Enemy.NineTailed.FoxFireVolley")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_EnhancedFoxFireVolley, "Enemy.NineTailed.EnhancedFoxFireVolley")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_FoxFireRandDrop, "Enemy.NineTailed.FoxFireRandDrop")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_FoxFireDrop, "Enemy.NineTailed.FoxFireDrop")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_PortalBite, "Enemy.NineTailed.PortalBite")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_PortalBreath, "Enemy.NineTailed.PortalBreath")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_PortalBreathCombo, "Enemy.NineTailed.PortalBreathCombo")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_FoxFireGroggy, "Enemy.NineTailed.FoxFireGroggy")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_CharmIllusion, "Enemy.NineTailed.CharmIllusion")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_EnhancedCharmIllusion, "Enemy.NineTailed.EnhancedCharmIllusion")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_NineTailed_ClawSwipe, "Enemy.NineTailed.ClawSwipe")
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

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP")
#pragma endregion

#pragma region Cost Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cooldown_Potion,  "Cooldown.Potion")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cooldown_Ammo,  "Cooldown.Ammo")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cooldown_Rolling, "Cooldown.Rolling")
#pragma endregion

#pragma region Equip Slots
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::EquipSlot_Weapon, "EquipSlot.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::EquipSlot_Ammo, "EquipSlot.Ammo")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::EquipSlot_Potion, "EquipSlot.Potion")
#pragma endregion
	
#pragma region Item Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Equip, "Item.Equip")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Equip_Weapon_NormalSword, "Item.Equip.Weapon.NormalSword")
	
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable, "Item.Consumable")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_HP, "Item.Consumable.HP")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_HP_SmallPotion, "Item.Consumable.HP.SmallPotion")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_XP, "Item.Consumable.XP")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_XP_SmallXP, "Item.Consumable.XP.SmallXP")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_XP_MidXP, "Item.Consumable.XP.MidXP")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_XP_LargeXP, "Item.Consumable.XP.LargeXP")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_Gold, "Item.Consumable.Gold")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_Gold_SmallGold, "Item.Consumable.Gold.SmallGold")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_Ammo, "Item.Consumable.Ammo")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Item_Consumable_Ammo_NormalAmmo, "Item.Consumable.Ammo.NormalAmmo")
#pragma endregion

#pragma region Dungeon Map Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Dungeon_Region_1, "Dungeon.Region.1")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Dungeon_Region_2, "Dungeon.Region.2")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Dungeon_Region_3, "Dungeon.Region.3")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Dungeon_Region_4, "Dungeon.Region.4")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Dungeon_Region_5, "Dungeon.Region.5")
#pragma endregion

#pragma region Gameplay Cue
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::GameplayCue_HitReact, "GameplayCue.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::GameplayCue_NineTailed_FoxFireVolley, "GameplayCue.NineTailed.FoxFireVolley")
#pragma endregion

#pragma region Difficulty Level
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::DifficultyLevel_Easy, "DifficultyLevel.Easy")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::DifficultyLevel_Normal, "DifficultyLevel.Normal")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::DifficultyLevel_Hard, "DifficultyLevel.Hard")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::DifficultyLevel_Expert, "DifficultyLevel.Expert")
#pragma endregion

#pragma region Cinematic Category
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_GameOpening, "Cinematic.GameOpening")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_WildBoar_Encounter, "Cinematic.WildBoar.Encounter")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_WildBoar_Defeat, "Cinematic.WildBoar.Defeat")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_BreakBarrier, "Cinematic.BreakBarrier")
	
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_NineTailed_Encounter, "Cinematic.NineTailed.Encounter")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_NineTailed_Phase2, "Cinematic.NineTailed.Phase2")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cinematic_NineTailed_Defeat, "Cinematic.NineTailed.Defeat")
#pragma endregion

#pragma region FX HitReact Tags
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Slash_Light, "FX.Type.Slash.Light")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Slash_Heavy, "FX.Type.Slash.Heavy")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Blunt_Light, "FX.Type.Blunt.Light")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Blunt_Heavy, "FX.Type.Blunt.Heavy")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Pierce_Light, "FX.Type.Pierce.Light")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Pierce_Heavy, "FX.Type.Pierce.Heavy")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Magic_Fire, "FX.Type.Magic.Fire")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::FX_Type_Magic_Heavy, "FX.Type.Magic.Heavy")
#pragma endregion

#pragma region BGM Category
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_Title, "BGM.Title")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_Hideout, "BGM.Hideout")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_CombatField_Default, "BGM.CombatField.Default")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_Combat_Default, "BGM.Combat.Default")
	
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_NineTailed_Phase1, "BGM.NineTailed.Phase1")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_NineTailed_Phase2, "BGM.NineTailed.Phase2")
	
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::BGM_WildBoar_Battle, "BGM.WildBoar.Battle")
#pragma endregion
}