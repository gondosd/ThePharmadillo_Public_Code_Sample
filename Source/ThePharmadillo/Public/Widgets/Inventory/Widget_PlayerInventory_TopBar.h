// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_PlayerInventory_TopBar.generated.h"


class UGD_Item_DataAsset;
class UGD_InventoryComponent;
class UInputAction;
class UWidget_Inventory;

UCLASS(Abstract)
class THEPHARMADILLO_API UWidget_PlayerInventory_TopBar : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OpenVisuals_BP();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CloseVisuals_BP();
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	void BlinkInventory();
	
private:
	UFUNCTION()
	void InventoryUpdated(UGD_InventoryComponent* BroadcastingComponent, UGD_Item_DataAsset* ItemThatChanged);
	
	void CloseInventoryFromTimer();

public:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> CloseInventoryAction;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InventoryOpenTime = 1.5f;

protected:
	FTimerHandle OpenTimeHandle;
	
private:
#pragma region Bindigns

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidget_Inventory> InventoryWidget;

#pragma endregion

};
