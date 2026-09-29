// Gondos Daniel all rights reserved.


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "FrontendDebugHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_ConfirmScreen.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResetAction)
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true,
			                  FSimpleDelegate::CreateUObject(
				                  this, &UWidget_OptionsScreen::OnResetBoundActionTriggered)));
	}


	RegisterUIActionBinding(
		FBindUIActionArgs(ICommonInputModule::GetSettings().GetEnhancedInputBackAction(), true,
		                  FSimpleDelegate::CreateUObject(
			                  this, &UWidget_OptionsScreen::OnBackBoundActionTriggered)));

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &UWidget_OptionsScreen::OnOptionsTabSelected);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &UWidget_OptionsScreen::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &UWidget_OptionsScreen::OnListViewItemSelected);
	
	CommonInputSubsystem = UCommonInputSubsystem::Get(GetOwningLocalPlayer());
	check(CommonInputSubsystem);
	CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::EvaluateResetAllState);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	TabListWidget_OptionsTabs->SetVisibility(ESlateVisibility::Visible);

	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}
		const FName TabID = TabCollection->GetDataID();
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))
		{
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	TabListWidget_OptionsTabs->SetVisibility(ESlateVisibility::Collapsed);

	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedEntryWidget;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("Data registry for options screen is not valid"));

	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());
	const FString SelectedTabButtonName = CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Warning!")),
		FText::FromString(TEXT("Are you sure you want to reset every current setting under the ") + SelectedTabButtonName + TEXT(" tab?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}
			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}
				if (DataToReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
				}
			}
			
			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}
			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}
	return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifiedReason ModifiedReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();

	CachedFoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems(CachedFoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems())
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	EvaluateResetAllState(CommonInputSubsystem->GetCurrentInputType());
}

void UWidget_OptionsScreen::EvaluateResetAllState(ECommonInputType CommonInput)
{
	ResettableDataArray.Empty();
	for (UListDataObject_Base* FoundListSourceItem : CachedFoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InHoveredItem),
			TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
				SelectedItem,
				TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UListDataObject_Base>(InSelectedItem),
		TryGetEntryWidgetClassName(InSelectedItem));
}
