// Gondos Daniel all rights reserved.


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/FrontendHapticsSubsystem.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
		UFrontendHapticsSubsystem::Get(GetOwningLocalPlayer())->PlayDefaultWeakFeedbackEffect(ERumbleType::Menu);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemSelected() ? true : false);
	}
}

void UWidget_ListEntry_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonInputSubsystem = UCommonInputSubsystem::Get(GetOwningLocalPlayer());
	check(CommonInputSubsystem);
	CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::OnInputTypeChanged);


	if (ResetAction)
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true,
			                  FSimpleDelegate::CreateUObject(
				                  this, &ThisClass::ResetOwningDataObject)));
	}
}

void UWidget_ListEntry_Base::ResetOwningDataObject()
{
	

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Warning!")),
		FText::FromString(TEXT("Are you sure you want to reset the ") + CachedOwningDataObject->GetDataDisplayName().ToString() + TEXT(" item?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			if (CachedOwningDataObject->TryResetBackToDefaultValue())
			{
				RemoveActionBinding(ResetActionHandle);
			}
		}
	);
}

void UWidget_ListEntry_Base::NativeOnItemSelectionChanged(bool bIsSelected)
{
	bIsThisEntryWidgetSelected = bIsSelected;
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	BP_OnToggleEntryWidgetHighlightState(bIsSelected);

	EvaluateResetBindingVisibility();
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));

	BP_OnToggleEntryWidgetHighlightState(false);
}

void UWidget_ListEntry_Base::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);

	bIsThisEntryWidgetSelected = false;
	EvaluateResetBindingVisibility();
}

FReply UWidget_ListEntry_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	//UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
	if (!InOwningListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnDependencyDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	}

	OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());
	CachedOwningDataObject = InOwningListDataObject;

	EvaluateResetBindingVisibility();
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason)
{
	EvaluateResetBindingVisibility();
	UFrontendHapticsSubsystem::Get(GetOwningLocalPlayer())->PlayDefaultWeakFeedbackEffect(ERumbleType::Menu);
}

void UWidget_ListEntry_Base::OnInputTypeChanged(ECommonInputType CommonInput)
{
	EvaluateResetBindingVisibility();
}

void UWidget_ListEntry_Base::OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData,
                                                                  EOptionsListDataModifiedReason ModifiedReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}

	EvaluateResetBindingVisibility();
}

void UWidget_ListEntry_Base::OnToggleEditableState(bool bIsEditable)
{
	SetIsInteractable(bIsEditable);

	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UWidget_ListEntry_Base::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}

void UWidget_ListEntry_Base::EvaluateResetBindingVisibility()
{
	if (CachedOwningDataObject && CachedOwningDataObject->CanResetBackToDefaultValue() && ResetAction && bIsThisEntryWidgetSelected && GetIsInteractable())
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
