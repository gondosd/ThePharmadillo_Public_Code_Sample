// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "GD_InventoryButton.generated.h"

class UGD_InventoryComponent;
class UGD_Item_DataAsset;

UCLASS()
class THEPHARMADILLO_API UGD_InventoryButton : public UFrontendCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetItem(UGD_Item_DataAsset* InSlotItem, const bool bIsEnabledSlot = true);
	
protected:	

	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//Being Called after the SlotItem is being changed
	UFUNCTION(BlueprintImplementableEvent)
	void RefreshItemVisuals_BP(UGD_Item_DataAsset* InSlotItem);
	
	
	//checks for the item, and displays actionwidget visibility and input handling
	virtual void EvaluateHover();
	virtual void UnregisterAllInputBindigns();
	
public:
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UGD_InventoryComponent> OwningInventoryComponent = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UGD_InventoryComponent> LinkedInventoryComponent = nullptr;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGD_Item_DataAsset> SlotItem;
	
};
