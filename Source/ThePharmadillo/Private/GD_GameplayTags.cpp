// Gondos Daniel all rights reserved.


#include "GD_GameplayTags.h"


namespace GD_GameplayTags
{
	namespace UI
	{
		namespace Stacks
		{
			//Frontend widget stack
			UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHud, "Frontend.WidgetStack.GameHud");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");
		}

		namespace Widgets
		{
			//Frontend widgets
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_OptionsScreen, "Frontend.Widget.OptionsScreen");
	
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_KeyRemapScreen, "Frontend.Widget.KeyRemapScreen");
	
			//In-game Widgets
			UE_DEFINE_GAMEPLAY_TAG(Game_Widget_MenuScreen, "Frontend.Widget.GameMenuScreen");
			UE_DEFINE_GAMEPLAY_TAG(Game_Widget_DialogueScreen, "Frontend.Widget.DialogueScreen");
		}
		
		namespace Images
		{
			//Frontend Options Image
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_TestImage, "Frontend.Image.TestImage");
			UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_BrightnessSetter, "Frontend.Image.BrightnessSetter");
		}
	
	}
	
}
