// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

UENUM(BlueprintType)
enum class EMovementInput : uint8
{
	Forward,
	Right,
	Backward,
	Left,

	End
};

UENUM()
enum class EHawnmunConfirmType : uint8
{
	Yes,
	No,

	End
};