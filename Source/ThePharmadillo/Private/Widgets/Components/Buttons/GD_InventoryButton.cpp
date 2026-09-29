// Gondos Daniel all rights reserved.


#include "Widgets/Components/Buttons/GD_InventoryButton.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UGD_InventoryButton::SetItem(UGD_Item_DataAsset* InSlotItem, const bool bIsEnabledSlot)
{
	SlotItem = InSlotItem;
	SetIsEnabled(bIsEnabledSlot);
	RefreshItemVisuals_BP(InSlotItem);
	EvaluateHover();
}

void UGD_InventoryButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	EvaluateHover();
}

void UGD_InventoryButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	UnregisterAllInputBindigns();
}

void UGD_InventoryButton::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
	
	SetFocus(); //TODO Is this necessary? has to be ckecked after the keyboard and gamepad handling is polished
}

FReply UGD_InventoryButton::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FEventReply Reply = UWidgetBlueprintLibrary::Unhandled();
		return UWidgetBlueprintLibrary::DetectDrag(Reply, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UGD_InventoryButton::EvaluateHover()
{
	UnregisterAllInputBindigns();
}

void UGD_InventoryButton::UnregisterAllInputBindigns()
{
}
