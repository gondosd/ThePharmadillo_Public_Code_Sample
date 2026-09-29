// Gondos Daniel all rights reserved.


#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "FrameworkClasses/GD_PlayerController.h"

void UWidget_ActivatableWidgetBase::CloseWidget()
{
	DeactivateWidget();
	OnWidgetClosed.Broadcast(this);
}

bool UWidget_ActivatableWidgetBase::NativeOnHandleBackAction()
{
	OnWidgetClosed.Broadcast(this);
	return Super::NativeOnHandleBackAction();
}

AGD_PlayerController* UWidget_ActivatableWidgetBase::GetOwningFrontendPlayerController()
{
	if (!CachedOwningFrontendPC.IsValid())
	{
		CachedOwningFrontendPC = GetOwningPlayer<AGD_PlayerController>();
	}

	return CachedOwningFrontendPC.IsValid() ? CachedOwningFrontendPC.Get() : nullptr;
}
