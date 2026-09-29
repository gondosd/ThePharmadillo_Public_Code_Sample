// Gondos Daniel all rights reserved.


#include "Widgets/Options/ListEntries/Widget_ListEntry_String.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "FrontendDebugHelper.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]() { SelectThisEntryWidget(); });
}

FReply UWidget_ListEntry_String::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{	
	if (GetIsInteractable())
	{
		if (InKeyEvent.GetKey() == FKey("Right"))
		{
			OnNextOptionButtonClicked();
			return FReply::Handled();
		}

		if (InKeyEvent.GetKey() == FKey("Left"))
		{
			OnPreviousOptionButtonClicked();
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifiedReason);
	
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	CommonButton_PreviousOption->SetIsEnabled(bIsEditable);
	CommonButton_NextOption->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject && GetIsInteractable())
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	if (CachedOwningStringDataObject && GetIsInteractable())
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnRotatorValueChanged(int Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}
	
//CommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());
	}
}
