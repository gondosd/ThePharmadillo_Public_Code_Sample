// Gondos Daniel all rights reserved.


#include "Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"

#include "CommonInputSubsystem.h"
#include "GD_GameplayTags.h"
#include "PharmadilloFunctionLibrary.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	OnInputTypeChanged(CommonInputSubsystem->GetCurrentInputType());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifiedReason);

	OnInputTypeChanged(CommonInputSubsystem->GetCurrentInputType());
}

void UWidget_ListEntry_KeyRemap::OnInputTypeChanged(ECommonInputType CommonInput)
{
	Super::OnInputTypeChanged(CommonInput);

	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey(CommonInput));
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	if (GetIsInteractable())
	{
		UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
			GD_GameplayTags::UI::Stacks::Frontend_WidgetStack_Modal,
			UPharmadilloFunctionLibrary::GetFrontendSoftWidgetClassByTag(GD_GameplayTags::UI::Widgets::Frontend_Widget_KeyRemapScreen),
			[this](EAsyncPushWidgetState PushState, UWidget_ActivatableWidgetBase* PushedWidget)
			{
				if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
				{
					UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
					CreatedKeyRemapScreen->SetEquippedBindingIcon(CommonButton_RemapKey->GetButtonDisplayImage());
					CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
					CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

					if (CommonInputSubsystem)
					{
						CreatedKeyRemapScreen->SetDesiredInputType(CommonInputSubsystem->GetCurrentInputType());
					}
				}
			}
		);
	}
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject)
		return;

	//check if the current key is already the default key. Display an OK screen that says that this is already the default key to the player

	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
			EConfirmScreenType::Ok,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(
				TEXT("The key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("is already set to default.")),
			[](EConfirmScreenButtonType ClickedButton)
			{
			});
		return;
	}

	//Reset the key binding back to default
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(
			TEXT("Are you sure you want to reset the key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() +
			TEXT("to default?")),
		[this](EConfirmScreenButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmScreenButtonType::Confirmed)
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
		}

	);
}

void UWidget_ListEntry_KeyRemap::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);
	
	CommonButton_RemapKey->SetIsEnabled(bIsEditable);
	CommonButton_ResetKeyBinding->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey, CommonInputSubsystem->GetCurrentInputType());
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("Key Remap")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButton)
		{
		}
	);
}
