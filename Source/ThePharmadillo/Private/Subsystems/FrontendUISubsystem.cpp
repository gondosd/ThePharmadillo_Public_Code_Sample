// Gondos Daniel all rights reserved.


#include "Subsystems/FrontendUISubsystem.h"

#include "PharmadilloFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "GD_GameplayTags.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Widgets/Widget_ConfirmScreen.h"
#include "Widgets/Widget_DialogueScreen.h"
#include "Widgets/HUD/Widget_HUD.h"


UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;

	//broadcast if the game has gameplay focus 
	InCreatedWidget->GetOwningLocalPlayer()->GetSubsystem<UCommonUIActionRouterBase>()->OnActiveInputModeChanged().AddUObject
		(this, &UFrontendUISubsystem::HandleActiveInputModeChanged);
}

void UFrontendUISubsystem::HandleActiveInputModeChanged(ECommonInputMode CommonInputMode)
{
	OnGameFocusChangedDelegate.Broadcast(CommonInputMode != ECommonInputMode::Menu);
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
                                                      TSoftClassPtr<UWidget_ActivatableWidgetBase> InSoftWidgetClass,
                                                      TFunction<void(EAsyncPushWidgetState,
                                                                     UWidget_ActivatableWidgetBase*)>
                                                      AsyncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass && CreatedPrimaryLayout);

				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(
					InWidgetStackTag);

				UWidget_ActivatableWidgetBase* CreatedWidget = FoundWidgetStack->AddWidget<
					UWidget_ActivatableWidgetBase>
				(
					LoadedWidgetClass,
					[AsyncPushStateCallback](UWidget_ActivatableWidgetBase& CreatedWidgetInstance)
					{
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UFrontendUISubsystem::PushConfirmScreenToModalStackAsynch(EConfirmScreenType InScreenType,
                                                               const FText& InScreenTitle, const FText& InScreenMsg,
                                                               TFunction<void(EConfirmScreenButtonType)>
                                                               ButtonClickedCallback, bool bShouldPauseGame)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;

	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::OKCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::YesNoCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoCancelScreen(InScreenTitle, InScreenMsg);
		break;
	default: ;
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(
		GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_Modal,
		UPharmadilloFunctionLibrary::GetFrontendSoftWidgetClassByTag(GD_GameplayTags::UI::Widgets::Frontend_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback, bShouldPauseGame](EAsyncPushWidgetState InPushState,
		                                                             UWidget_ActivatableWidgetBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UWidget_ConfirmScreen>(PushedWidget);

				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback, bShouldPauseGame);
			}
		});
}

void UFrontendUISubsystem::PushDialogueScreenToModalStackAsync(const UGD_DialogueDataAsset& InDialogueDataAsset,
                                                               TFunction<void(FName)> ButtonClickedCallback)
{
	UDialogueScreenInfoObject* CreatedInfoObject = nullptr;
	CreatedInfoObject = UDialogueScreenInfoObject::CreateDialogueScreen(InDialogueDataAsset);

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(
		GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_Modal,
		UPharmadilloFunctionLibrary::GetFrontendSoftWidgetClassByTag(GD_GameplayTags::UI::Widgets::Game_Widget_DialogueScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState,
		                                           UWidget_ActivatableWidgetBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_DialogueScreen* CreatedConfirmScreen = CastChecked<UWidget_DialogueScreen>(PushedWidget);

				CreatedConfirmScreen->InitDialogueScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		});
}

void UFrontendUISubsystem::ClearEntireWidgetStackByTag(const FGameplayTag InTag) const
{
	if (UCommonActivatableWidgetContainerBase* FoundStack = CreatedPrimaryLayout->FindWidgetStackByTag(InTag))
	{
		FoundStack->ClearWidgets();
	}
}

bool UFrontendUISubsystem::CheckWidgetStackIfEmptyByTag(const FGameplayTag InTag) const
{
	if (const UCommonActivatableWidgetContainerBase* FoundStack = CreatedPrimaryLayout->FindWidgetStackByTag(InTag))
	{
		return FoundStack->GetNumWidgets() > 0;
	}
	return false;
}
