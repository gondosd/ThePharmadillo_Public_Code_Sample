// Gondos Daniel all rights reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace GD_GameplayTags
{
	namespace UI
	{
		namespace Stacks
		{
			//Frontend widget stack
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHud);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);
		}

		namespace Widgets
		{
			//Frontend widgets
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_OptionsScreen);

			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_KeyRemapScreen);

			//In-game Widgets
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Widget_MenuScreen);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Game_Widget_DialogueScreen);
		}
		
		namespace Images
		{
			//Frontend Options Image
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Image_TestImage);
			THEPHARMADILLO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Image_BrightnessSetter);
		}
	}
}
