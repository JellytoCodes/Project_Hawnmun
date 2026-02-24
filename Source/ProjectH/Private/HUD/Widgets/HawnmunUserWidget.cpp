// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "HUD/Widgets/HawnmunUserWidget.h"

void UHawnmunUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
