// Gondos Daniel all rights reserved.


#include "FrameworkClasses/GD_GameplayPlayerController.h"

#include "PharmadilloFunctionLibrary.h"
#include "GD_GameplayTags.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "Subsystems/FrontendAudioSubsystem.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"

void AGD_GameplayPlayerController::TogglePauseMenu()
{
	if (UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this))
	{
		if (FrontendUISubsystem->CheckWidgetStackIfEmptyByTag(GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_GameMenu))
		{
			FrontendUISubsystem->ClearEntireWidgetStackByTag(GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_GameMenu);
			SetPause(false);
			UFrontendAudioSubsystem::Get(this)->PauseMusic(false);
		}
		else
		{
			TSoftClassPtr<UWidget_ActivatableWidgetBase> PauseMenuWidget = UPharmadilloFunctionLibrary::GetFrontendSoftWidgetClassByTag(
				GD_GameplayTags::UI::Widgets::Game_Widget_MenuScreen);
			FrontendUISubsystem->PushSoftWidgetToStackAsync(GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_GameMenu,
			                                                PauseMenuWidget,
			                                                [this](EAsyncPushWidgetState AsyncPushWidgetState,
			                                                       UWidget_ActivatableWidgetBase* Widget_ActivatableWidgetBase)
			                                                {
				                                                Widget_ActivatableWidgetBase->OnWidgetClosed.AddUObject(
					                                                this, &AGD_GameplayPlayerController::PausePanelClosed);
			                                                }
			);
			SetPause(true);
			UFrontendAudioSubsystem::Get(this)->PauseMusic(true);
		}
	}
}

void AGD_GameplayPlayerController::OpenInventory()
{
	if (InventoryWidget && !InventoryWidget->IsActivated())
		InventoryWidget->ActivateWidget();
}

void AGD_GameplayPlayerController::PausePanelClosed(UWidget_ActivatableWidgetBase* Widget_ActivatableWidgetBase)
{
	SetPause(false);
	UFrontendAudioSubsystem::Get(this)->PauseMusic(false);
}
