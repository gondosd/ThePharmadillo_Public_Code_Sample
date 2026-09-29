// Gondos Daniel all rights reserved.


#include "Widgets/Components/Buttons/GD_PlayerInventoryButton.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "DataAssets/GD_Item_DataAsset.h"
#include "Input/CommonUIInputTypes.h"

void UGD_PlayerInventoryButton::EvaluateHover()
{
	Super::EvaluateHover();

	HandleConsumable();
	HandleDroppable();
}

void UGD_PlayerInventoryButton::HandleConsumable()
{
	if (!SlotItem || SlotItem->GameplayEffect.IsNull()) return; //the item has no gameplay effect, meaning, its not consumable


	if (ConsumeItemAction && IsHovered())
	{
		ConsumeItemBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ConsumeItemAction, true,
			                  FSimpleDelegate::CreateUObject(this, &ThisClass::ConsumeItem)));
	}
}

void UGD_PlayerInventoryButton::HandleDroppable()
{
	if (!SlotItem) return; //if slot is empty, there is nothing to drop

	if (DropItemAction && IsHovered())
	{
		DropItemBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(DropItemAction, true,
			                  FSimpleDelegate::CreateUObject(this, &ThisClass::DropItem)));
	}
}

void UGD_PlayerInventoryButton::ConsumeItem() const
{
	check(OwningInventoryComponent);
	OwningInventoryComponent->ConsumeItem(SlotItem);
}

void UGD_PlayerInventoryButton::DropItem() const
{
	check(OwningInventoryComponent);
	OwningInventoryComponent->DropItem(SlotItem);
}

void UGD_PlayerInventoryButton::UnregisterAllInputBindigns()
{
	Super::UnregisterAllInputBindigns();

	ConsumeItemBindingHandle.Unregister();
	DropItemBindingHandle.Unregister();
}
