// Gondos Daniel all rights reserved.


#include "Widgets/Inventory/Widget_StorageInventory.h"

#include "CommonInputSubsystem.h"
#include "FrontendDebugHelper.h"
#include "Components/GridPanel.h"
#include "Groups/CommonButtonGroupBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Inventory/Widget_Inventory.h"
#include "Widgets/Inventory/Widget_PlayerInventory_TopBar.h"

void UWidget_StorageInventory::NativeConstruct()
{
	Super::NativeConstruct();
	bAutomaticallyRegisterInputOnConstruction = true;
}

void UWidget_StorageInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetOwningLocalPlayer()))
	{
		// only display input binding in ActionBar if we use gamepad
		const bool bShouldDisplay = InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;

		if (FocusToPlayerInventoryAction)
			FocusToPlayerInventoryBindingHandle = RegisterUIActionBinding(
				FBindUIActionArgs(FocusToPlayerInventoryAction, bShouldDisplay,
				                  FSimpleDelegate::CreateUObject(this, &ThisClass::FocusPlayerInventory)));

		if (FocusToStorageAction)
			FocusToStorageBindingHandle = RegisterUIActionBinding(
				FBindUIActionArgs(FocusToStorageAction, bShouldDisplay,
				                  FSimpleDelegate::CreateUObject(this, &ThisClass::FocusStorageInventory)));
		
		//Dynamic display input binding in ActionBar on inputmethod change
		UCommonInputSubsystem::Get(GetOwningLocalPlayer())->OnInputMethodChangedNative.AddLambda([this](ECommonInputType InputType)
		{
			const bool bShouldDisplay = InputType == ECommonInputType::Gamepad;
			FocusToPlayerInventoryBindingHandle.SetDisplayInActionBar(bShouldDisplay);
			FocusToStorageBindingHandle.SetDisplayInActionBar(bShouldDisplay);
		});
	}
}

void UWidget_StorageInventory::NativeOnActivated()
{
	Super::NativeOnActivated();

	SelectInventory(ActiveInventory);
}

UWidget_Inventory* UWidget_StorageInventory::GetActiveInventoryWidget() const
{
	switch (ActiveInventory)
	{
	case EActiveInventoryType::PlayerInventory:
		return PlayerInventoryWidget;
	case EActiveInventoryType::Storage:
		return StorageInventoryWidget;
	default: Debug::Print(("SelectInventory switch function is not prepared for this case"));
		return nullptr;
	}
}

void UWidget_StorageInventory::SelectInventory(EActiveInventoryType InventoryType)
{
	ActiveInventory = InventoryType;
	GetActiveInventoryWidget()->GridPanel->GetChildAt(0)->SetFocus();
}

void UWidget_StorageInventory::FocusPlayerInventory()
{
	SelectInventory(EActiveInventoryType::PlayerInventory);
}

void UWidget_StorageInventory::FocusStorageInventory()
{
	SelectInventory(EActiveInventoryType::Storage);
}
