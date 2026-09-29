// Gondos Daniel all rights reserved.


#include "FrameworkClasses/GD_HUD.h"

#include "GD_GameplayTags.h"
#include "FrameworkClasses/GD_GameplayPlayerController.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/Credits/Widget_CreditsScreen.h"
#include "Widgets/HUD/Widget_HUD.h"

void AGD_HUD::OnLoadingScreenDeactivated_Implementation()
{
	IFrontendLoadingScreenInterface::OnLoadingScreenDeactivated_Implementation();
	
	checkf( PrimaryLayoutWidgetClass, TEXT("Primary Layout Widget is unset"));
	
	InCreatedWidget = CreateWidget<UWidget_PrimaryLayout>(GetOwningPlayerController(), PrimaryLayoutWidgetClass);
	InCreatedWidget->AddToViewport();
	UFrontendUISubsystem::Get(this)->RegisterCreatedPrimaryLayoutWidget(InCreatedWidget);
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
	
	
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this);
	if (FrontendUISubsystem && !HUDWidgetClass.IsNull())
	{
		FrontendUISubsystem->PushSoftWidgetToStackAsync(GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_GameHud,
														HUDWidgetClass, [this](EAsyncPushWidgetState AsyncPushWidgetState,
																	UWidget_ActivatableWidgetBase* Widget_ActivatableWidgetBase)
														{
															if (AsyncPushWidgetState == EAsyncPushWidgetState::OnCreatedBeforePush)
															{
																UWidget_ActivatableWidgetBase* InventoryWidget = Cast<UWidget_HUD>(Widget_ActivatableWidgetBase)
																	->GetInventoryWidgetReference();
			                                                	
																Cast<AGD_GameplayPlayerController>(GetOwningPlayerController())
																	->SetInventoryWidgetReference(InventoryWidget);
															}
														}
		);
	}
}
