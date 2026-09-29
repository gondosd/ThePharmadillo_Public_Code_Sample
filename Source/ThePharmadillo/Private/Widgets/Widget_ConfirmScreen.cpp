// Gondos Daniel all rights reserved.


#include "Widgets/Widget_ConfirmScreen.h"

#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString("Ok");
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
                                                                      const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString("Yes");
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Declined;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("No");
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(
	const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString("Ok");
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString("Cancel");
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoCancelScreen(const FText& InScreenTitle,
                                                                            const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString("Yes");
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Declined;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("No");
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	FConfirmScreenButtonInfo CancelButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("Cancel");
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
                                              TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback, bool bInShouldPauseGame)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	bShouldPauseGame = bInShouldPauseGame;

	//Checking if the entry box has old button created previously
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		/*
		 *Clearing the old buttons the entry box has. The widget type for the entry box 
		 *is specified in the child widget blueprint
		 */
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			});
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty())

	for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, AvailableButtonInfo, this]()
			{
				ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);
				DeactivateWidget();
			});
	}
}

void UWidget_ConfirmScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	bWasGamePausedBeforeOpening = GetOwningPlayer()->IsPaused();

	if (bShouldPauseGame)
		GetOwningPlayer()->SetPause(true);
}

void UWidget_ConfirmScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (!bWasGamePausedBeforeOpening)
		GetOwningPlayer()->SetPause(false);
}

UWidget* UWidget_ConfirmScreen::NativeGetDesiredFocusTarget() const
{
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		/*
		 *Set focus on the last button. So if there are two buttons, one is Yes, one is No.
		 *our gamepad will focus on the No button
		 */
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}

	return Super::NativeGetDesiredFocusTarget();
}

bool UWidget_ConfirmScreen::NativeOnHandleBackAction()
{
	CastChecked<UCommonButtonBase>(DynamicEntryBox_Buttons->GetAllEntries().Last())->OnClicked().Broadcast();

	return true;
}
