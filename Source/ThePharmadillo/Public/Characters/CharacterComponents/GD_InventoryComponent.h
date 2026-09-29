// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAssets/GD_Item_DataAsset.h"
#include "GD_InventoryComponent.generated.h"


class UCommonActivatableWidget;
class UFrontendDeveloperSettings;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, UGD_InventoryComponent*, BroadcastingComponent, UGD_Item_DataAsset*, ItemThatChanged);

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(SaveGame)
	UGD_Item_DataAsset* ItemData = nullptr;

	UPROPERTY(SaveGame)
	int32 Quantity = 0;
	
	FORCEINLINE bool IsEmpty() const { return ItemData == nullptr; }
	
	bool operator==(const UGD_Item_DataAsset* OtherData) const
	{
		return ItemData == OtherData;
	}
	
	bool operator==(const FInventorySlot& Other) const
	{
		return ItemData == Other.ItemData;
	}
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPHARMADILLO_API UGD_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UGD_InventoryComponent();
	virtual void BeginPlay() override;

	/// Consumes Item if its consumable
	/// @param ItemToConsume Item type to consume
	/// @param Amount How many you consume, negative number means all
	UFUNCTION(BlueprintCallable)
	bool ConsumeItem(UGD_Item_DataAsset* ItemToConsume, const int32 Amount = 1);
	/// Adds Item to the current inventory
	/// @param ItemToAdd Item type to add
	/// @param Amount How many you add, negative number means infinite
	UFUNCTION(BlueprintCallable)
	bool AddItem(UGD_Item_DataAsset* ItemToAdd, const int32 Amount = 1);
	/// Gives you Infinite amount of the item
	/// @param ItemToAdd Item type to add infinitely
	UFUNCTION(BlueprintCallable)
	void GrantInfiniteItem(UGD_Item_DataAsset* ItemToAdd);
	/// Removes Item from the current inventory
	/// @param ItemToRemove Item type to remove
	/// @param Amount How many you want to remove. If you want to subtract more than we have, it will be removed. Negative number means infinite, it removes all instance
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UGD_Item_DataAsset* ItemToRemove, const int32 Amount = 1);
	/// Drops Item on the ground
	/// @param ItemToDrop Item type to drop
	/// @param Amount How many you drop, negative number means all
	/// @param bForceDropItem it will drop the item even if its not in the inventory currently
	UFUNCTION(BlueprintCallable)
	bool DropItem(UGD_Item_DataAsset* ItemToDrop, const int32 Amount = 1, bool bForceDropItem = false);
	/// Swaps Slot contents in the inventories
	/// @param SourceIndex ItemSlot to transfer from the caller inventory
	/// @param TargetInventoryComponent Target inventory ( it can be the same as the caller inventory)
	/// @param TargetIndex ItemSlot to transfer to in the target inventory, if -1, finds the first empty place
	UFUNCTION(BlueprintCallable)
	bool TransferSlots(const int32 SourceIndex,UGD_InventoryComponent* TargetInventoryComponent,  const int32 TargetIndex = -1);
	/// Moves Slot contents to the linked inventories
	/// @param SourceIndex ItemSlot to transfer from the caller inventory
	/// @param TargetInventoryComponent Target inventory
	UFUNCTION(BlueprintCallable)
	bool AutoMoveSlot(const int32 SourceIndex,UGD_InventoryComponent* TargetInventoryComponent);
	///Wipes the whole inventory
	UFUNCTION(BlueprintCallable)
	void FlushInventory();

	/// Shows how many instance there is in the inventory from the given item type
	/// @param ItemToCheck Item type to check, if its null we get all item types count
	/// @return the amount of the instances of the given item type
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetNumberOfItemsInInventory(UGD_Item_DataAsset* ItemToCheck) const;
	/// Checks if Item type exist in the Inventory
	/// @param ItemToCheck Item type to inspect
	/// @param OutCount How many instance you have from the given item type
	/// @return returns true if the item exists in the inventory
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsItemInInventory(UGD_Item_DataAsset* ItemToCheck, int32& OutCount) const;
	/// Gets all Item types which are present in the inventory
	/// @param OutNumberOfItemTypes how many types we can find
	/// @return The actual array of the itemtypes
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UGD_Item_DataAsset*> GetAllItemTypesFromInventory(int32& OutNumberOfItemTypes) const;
	/// GetPlayerInventory map
	/// @return The whole inventory in Map form
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<FInventorySlot> GetInventory() const {return Inventory;}
	
	/// GetPlayerInventory map
	/// @return The whole inventory in Map form
	UFUNCTION(BlueprintCallable)
	void AddInventorySlot();
	
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 InventorySize = 10;
	
	UPROPERTY(SaveGame)
	TArray<FInventorySlot> Inventory;
	
	UPROPERTY(Transient)
	const UFrontendDeveloperSettings* CachedFrontendDeveloperSettings;
	
	
	void InventoryIsFullMessaging(UGD_Item_DataAsset* ItemToAdd, int32 Amount);
	
	void PlayAddSoundAsync(const TSoftObjectPtr<USoundBase> Sound) const;
};
