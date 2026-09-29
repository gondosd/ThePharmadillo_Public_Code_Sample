// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GD_InventoryButton.h"
#include "GD_StorageInventoryButton.generated.h"

/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UGD_StorageInventoryButton : public UGD_InventoryButton
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PushDeletionPrompt_BP();
	
protected:
	//checks for the item, and displays actionwidget visibility and input handling
	void EvaluateHover() override;
	
	void HandleTransferable();
	void HandleDeletable();
	
	void TransferItem();
	UFUNCTION(BlueprintCallable)
	void DeleteItem() const;
	
	void UnregisterAllInputBindigns() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> TransferItemAction;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> DeleteItemAction;
	
private:
	
	FUIActionBindingHandle TransferItemBindingHandle;
	FUIActionBindingHandle DeleteItemBindingHandle;
};
