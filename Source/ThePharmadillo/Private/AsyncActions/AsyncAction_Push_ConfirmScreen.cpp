// Gondos Daniel all rights reserved.


#include "AsyncActions/AsyncAction_Push_ConfirmScreen.h"

#include "Subsystems/FrontendUISubsystem.h"

UAsyncAction_Push_ConfirmScreen* UAsyncAction_Push_ConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                                    EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage,
                                                                                    bool bInShouldPauseGame)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_Push_ConfirmScreen* Node = NewObject<UAsyncAction_Push_ConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;
			Node->bShouldPauseGame = bInShouldPauseGame;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}

void UAsyncAction_Push_ConfirmScreen::Activate()
{
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());

	FrontendUISubsystem->PushConfirmScreenToModalStackAsynch(CachedScreenType, CachedScreenTitle, CachedScreenMessage,
	                                                         [this](EConfirmScreenButtonType ClickedButtonType)
	                                                         {
		                                                         OnButtonClicked.Broadcast(ClickedButtonType);

		                                                         SetReadyToDestroy();
	                                                         },
	                                                         bShouldPauseGame);
}
