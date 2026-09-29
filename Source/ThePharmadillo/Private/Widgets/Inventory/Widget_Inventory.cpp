// Gondos Daniel all rights reserved.


#include "Widgets/Inventory/Widget_Inventory.h"

#include "PharmadilloFunctionLibrary.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "Components/GridPanel.h"
#include "Groups/CommonButtonGroupBase.h"
#include "Misc/DataValidation.h"
#include "Widgets/Components/Buttons/GD_InventoryButton.h"

void UWidget_Inventory::SetInventoryComponents(UGD_InventoryComponent* InInventoryComponent, UGD_InventoryComponent* InLinkedInventoryComponent)
{
	if (UGD_InventoryComponent* OldInventoryComp = InventoryComponent.Get()) //cleanup if it was already set to a different one
		OldInventoryComp->OnInventoryUpdated.RemoveDynamic(this, &ThisClass::InventoryUpdated);


	InventoryComponent = InInventoryComponent;
	if (InInventoryComponent)
		InInventoryComponent->OnInventoryUpdated.AddDynamic(this, &ThisClass::InventoryUpdated);
	
	LinkedInventoryComponent = InLinkedInventoryComponent;
	
	PopulateGrid();
	
	RefreshInventoryVisuals();
}

void UWidget_Inventory::PopulateGrid(bool bIsDesignTime)
{
	if (RowSize <= 0) return; //invalid size
	
	if (!ButtonGroup)
		ButtonGroup = NewObject<UCommonButtonGroupBase>(this);
	ButtonGroup->SetSelectionRequired(false);
	
	GridPanel->ClearChildren();
	for (int i = 0; i < MaxDisplayedSlotCount; ++i)
	{
		UGD_InventoryButton* NewEntry = CreateWidget<UGD_InventoryButton>(this, SlotWidgetClass);

		if (!bIsDesignTime)
		{
			NewEntry->OwningInventoryComponent = GetInventoryComponent();
			NewEntry->LinkedInventoryComponent = GetLinkedInventoryComponent();
			NewEntry->SlotIndex = i;
			ButtonGroup->AddWidget(NewEntry);
		}
		GridPanel->AddChildToGrid(NewEntry, (i / RowSize), (i % RowSize));
	}	
}

void UWidget_Inventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	//fallback: if no inventory component is given, the player inventory is the default
	if (!GetInventoryComponent())
		SetInventoryComponents(UPharmadilloFunctionLibrary::GetPlayerInventoryComponent(this));
	
	PopulateGrid();

	RefreshInventoryVisuals();
}

void UWidget_Inventory::InventoryUpdated(UGD_InventoryComponent* BroadcastingComponent, UGD_Item_DataAsset* ItemThatChanged)
{
	RefreshInventoryVisuals();
}

void UWidget_Inventory::RefreshInventoryVisuals()
{
	TArray<UWidget*> SlotWidgets = GridPanel->GetAllChildren();
	const TArray<FInventorySlot>& Slots = GetInventoryComponent()->GetInventory();
	
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		UGD_InventoryButton* InventoryButton = CastChecked<UGD_InventoryButton>(SlotWidgets[i]);
		InventoryButton->SetItem(Slots.IsValidIndex(i) ? Slots[i].ItemData : nullptr, Slots.IsValidIndex(i));
	}
}

#if WITH_EDITOR
EDataValidationResult UWidget_Inventory::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!SlotWidgetClass)
	{
		Context.AddError(FText::FromString(TEXT("SlotWidgetClass is unset! This Widget cannot be compiled empty.")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif
