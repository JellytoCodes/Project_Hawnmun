// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HawnmunGameplayTags.h"

namespace HawnmunGameplayTags
{
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

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Cost_Sprint, "Cost.Sprint")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_Rolling, "Ability.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_Sprint, "Ability.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Ability_TargetLock, "Ability.TargetLock")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Stamina_RegenBlocked, "State.Stamina.RegenBlocked")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Stamina_Full, "State.Stamina.Full")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Dead, "State.Dead")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_Rolling, "State.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::State_TargetLock, "State.TargetLock")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_HitReact, "Event.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_Invincible, "Event.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_SwitchTarget_Left, "Event.SwitchTarget.Left")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Event_SwitchTarget_Right, "Event.SwitchTarget.Right")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Strafing, "Enemy.State.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_UnderAttack, "Enemy.State.UnderAttack")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_State_Unblockable, "Enemy.State.Unblockable")

	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_Pig_MeleeAttack1, "Enemy.Pig.MeleeAttack1")
	UE_DEFINE_GAMEPLAY_TAG(HawnmunGameplayTags::Enemy_Pig_MeleeAttack2, "Enemy.Pig.MeleeAttack2")
}