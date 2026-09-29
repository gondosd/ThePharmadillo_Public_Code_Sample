// Gondos Daniel all rights reserved.


#include "Widgets/Inventory/Widget_PlayerInventory_TopBar.h"

#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "Components/GridPanel.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Inventory/Widget_Inventory.h"

void UWidget_PlayerInventory_TopBar::NativeConstruct()
{
	Super::NativeConstruct();
	bAutomaticallyRegisterInputOnConstruction = true;
}

void UWidget_PlayerInventory_TopBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (CloseInventoryAction)
		RegisterUIActionBinding(
			FBindUIActionArgs(CloseInventoryAction, false,
							  FSimpleDelegate::CreateUObject(this, &ThisClass::DeactivateWidget)));
	
	if (UGD_InventoryComponent* Inventory = InventoryWidget->GetInventoryComponent())
	{
		Inventory->OnInventoryUpdated.AddUniqueDynamic(this, &ThisClass::InventoryUpdated);
	}
}

void UWidget_PlayerInventory_TopBar::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	if (OpenTimeHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(OpenTimeHandle);
	}
	else
	{
		InventoryWidget->GridPanel->GetChildAt(0)->SetFocus();
		OpenVisuals_BP();
	}
}

void UWidget_PlayerInventory_TopBar::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	CloseVisuals_BP();
}

void UWidget_PlayerInventory_TopBar::BlinkInventory()
{
	if (IsActivated()) return; // no need to blink

	OpenVisuals_BP();

	GetWorld()->GetTimerManager().SetTimer(
		OpenTimeHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::CloseInventoryFromTimer),
		InventoryOpenTime,
		false);
}

void UWidget_PlayerInventory_TopBar::InventoryUpdated(UGD_InventoryComponent* BroadcastingComponent, UGD_Item_DataAsset* ItemThatChanged)
{
	BlinkInventory();
}

void UWidget_PlayerInventory_TopBar::CloseInventoryFromTimer()
{
	OpenTimeHandle.Invalidate();
	CloseVisuals_BP();
}