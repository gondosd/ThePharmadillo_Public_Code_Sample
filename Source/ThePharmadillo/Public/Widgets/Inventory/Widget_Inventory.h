// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Widget_Inventory.generated.h"

class UGD_InventoryButton;
class UGridPanel;
class UGD_Item_DataAsset;
class UGD_InventoryComponent;
class UCommonButtonGroupBase;
class UCommonButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySelectionChanged, int32, NewSelectedIndex);


UCLASS(Abstract)
class THEPHARMADILLO_API UWidget_Inventory : public UCommonUserWidget
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override; //Compiler error
#endif
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Display")
	void SetInventoryComponents(UGD_InventoryComponent* InInventoryComponent, UGD_InventoryComponent* InLinkedInventoryComponent = nullptr);

	UFUNCTION(BlueprintPure, Category = "Inventory|Display")
	FORCEINLINE UGD_InventoryComponent* GetInventoryComponent() const { return InventoryComponent.Get(); }
	
	UFUNCTION(BlueprintPure, Category = "Inventory|Transfer")
	FORCEINLINE UGD_InventoryComponent* GetLinkedInventoryComponent() const { return LinkedInventoryComponent.Get(); }


protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Layout")
	void PopulateGrid(bool bIsDesignTime = false);
	
	void RefreshInventoryVisuals();

private:
	UFUNCTION()
	void InventoryUpdated(UGD_InventoryComponent* BroadcastingComponent, UGD_Item_DataAsset* ItemThatChanged);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Layout")
	TSubclassOf<UGD_InventoryButton> SlotWidgetClass;

	//how many slot will be displayed (inactive ones included) //TODO: maybe this could be moved to the inventory component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Layout")
	int32 MaxDisplayedSlotCount = 10;

	//how many slot will fit in one row
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Layout")
	int32 RowSize = 10;

#pragma region Bindigns

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<UGridPanel> GridPanel;

#pragma endregion

private:
	UPROPERTY(Instanced, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonGroupBase> ButtonGroup;

	UPROPERTY(Transient)
	TWeakObjectPtr<UGD_InventoryComponent> InventoryComponent;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UGD_InventoryComponent> LinkedInventoryComponent;

	int32 SelectedSlotIndex = 0;
};
