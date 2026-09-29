// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GD_InventoryButton.h"
#include "GD_PlayerInventoryButton.generated.h"

UCLASS()
class THEPHARMADILLO_API UGD_PlayerInventoryButton : public UGD_InventoryButton
{
	GENERATED_BODY()

	//checks for the item, and displays actionwidget visibility and input handling
	void EvaluateHover() override;
	
	void HandleConsumable();
	void HandleDroppable();
	
	void ConsumeItem() const;
	void DropItem() const;
	
	void UnregisterAllInputBindigns() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ConsumeItemAction;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> DropItemAction;
	
private:
	
	FUIActionBindingHandle ConsumeItemBindingHandle;
	FUIActionBindingHandle DropItemBindingHandle;
	
};
