// Gondos Daniel all rights reserved.


#include "AsyncActions/GD_AsyncAction_Push_DialogueScreen.h"

#include "FrontendDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DataAssets/GD_DialogueDataAsset.h"
#include "Subsystems/FrontendUISubsystem.h"

UGD_AsyncAction_Push_DialogueScreen* UGD_AsyncAction_Push_DialogueScreen::PushDialogueScreen(const UObject* WorldContextObject,
                                                                                             const UGD_DialogueDataAsset* DialogueDataAsset,
                                                                                             const AActor* Instigator)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UGD_AsyncAction_Push_DialogueScreen* Node = NewObject<UGD_AsyncAction_Push_DialogueScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedDialogueDataAsset = DialogueDataAsset;
			Node->Instigator = Instigator;
			
			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}

void UGD_AsyncAction_Push_DialogueScreen::Activate()
{	
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	
	if (Instigator)
		SetConversationFlag(true);
	
	FrontendUISubsystem->PushDialogueScreenToModalStackAsync(*CachedDialogueDataAsset,
		[this](const FName ClickedAnswerID)
		{
			if (Instigator)
				SetConversationFlag(false);
			
			OnButtonClicked.Broadcast(ClickedAnswerID);
			
			SetReadyToDestroy();
		});
}

void UGD_AsyncAction_Push_DialogueScreen::SetConversationFlag(bool bValue) const
{
	if (const APawn* NPC = Cast<APawn>(Instigator))
	{
		if (const AController* Controller = NPC->GetController())
		{
			if (UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>())
			{
				BB->SetValueAsBool(FName("bConversation"), bValue);
			}
		}
	}
}
