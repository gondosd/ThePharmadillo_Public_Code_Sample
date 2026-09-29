// Gondos Daniel all rights reserved.


#include "Widgets/Components/Buttons/GD_StorageInventoryButton.h"

#include "FrontendDebugHelper.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "Input/CommonUIInputTypes.h"

void UGD_StorageInventoryButton::EvaluateHover()
{
	Super::EvaluateHover();
	
	HandleTransferable();
	HandleDeletable();
}

void UGD_StorageInventoryButton::HandleTransferable()
{
	if (!SlotItem) return; ////if slot is empty, there is nothing to transfer


	if (TransferItemAction && IsHovered())
	{
		TransferItemBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(TransferItemAction, true,
							  FSimpleDelegate::CreateUObject(this, &ThisClass::TransferItem)));
	}
}

void UGD_StorageInventoryButton::HandleDeletable()
{
	if (!SlotItem) return; //if slot is empty, there is nothing to delete

	if (DeleteItemAction && IsHovered())
	{
		DeleteItemBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(DeleteItemAction, true,
							  FSimpleDelegate::CreateUObject(this, &ThisClass::PushDeletionPrompt_BP)));
	}
}

void UGD_StorageInventoryButton::TransferItem()
{
	if (!SlotItem) return;
	
	check(OwningInventoryComponent);
	check(LinkedInventoryComponent);
	
	OwningInventoryComponent->TransferSlots(SlotIndex, LinkedInventoryComponent);
}

void UGD_StorageInventoryButton::DeleteItem() const
{
	if (!SlotItem) return;
	
	check(OwningInventoryComponent);
	OwningInventoryComponent->RemoveItem(SlotItem);
}

void UGD_StorageInventoryButton::UnregisterAllInputBindigns()
{
	Super::UnregisterAllInputBindigns();
	
	TransferItemBindingHandle.Unregister();
	DeleteItemBindingHandle.Unregister();
}
