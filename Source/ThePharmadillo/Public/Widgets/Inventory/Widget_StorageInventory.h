// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_StorageInventory.generated.h"

class UWidget_Inventory;
class UInputAction;

UCLASS()
class THEPHARMADILLO_API UWidget_StorageInventory : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWidget_Inventory* GetActiveInventoryWidget() const;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	
	void FocusPlayerInventory();
	void FocusStorageInventory();
	
private:
	void SelectInventory(EActiveInventoryType InventoryType);
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> FocusToPlayerInventoryAction;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> FocusToStorageAction;
	
private:
#pragma region Bindigns

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidget_Inventory> PlayerInventoryWidget;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidget_Inventory> StorageInventoryWidget;

#pragma endregion
	
	FUIActionBindingHandle FocusToPlayerInventoryBindingHandle;
	FUIActionBindingHandle FocusToStorageBindingHandle;
	
	UPROPERTY(Transient)
	EActiveInventoryType ActiveInventory = EActiveInventoryType::PlayerInventory;
	
};
