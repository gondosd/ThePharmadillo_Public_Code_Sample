// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "PharmadilloFunctionLibrary.generated.h"


class UGD_InventoryComponent;
class USaveSlot;
class USaveManager;
struct FInventorySlot;
class AGD_PlayerStateBase;
class UGD_Item_DataAsset;
class UWidget_ActivatableWidgetBase;


UCLASS()
class THEPHARMADILLO_API UPharmadilloFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region FrameWork Helpers
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static const AGD_PlayerStateBase* GetGD_PlayerState(const UObject* WorldContextObject, const int32 PlayerIndex = 0);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static USaveManager* GetSaveSubsystem(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Maps", meta = (WorldContext = "WorldContextObject"))
	static FString GetCurrentLevelNameWithoutPersistentPrefix(const UObject* WorldContextObject);
#pragma endregion

#pragma region DeveloperSettingsGetter
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UWidget_ActivatableWidgetBase> GetFrontendSoftWidgetClassByTag(
		UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Frontend.Image")) FGameplayTag InImageTag);
#pragma endregion

#pragma region Save/Load
	UFUNCTION(BlueprintCallable, Category = "save")
	static bool FileSaveString(FString StringToSave, FString FilePath);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "save")
	static bool FileLoadString(FString FilePath, FString& OutString);
	
	
	UFUNCTION(BlueprintCallable, Category = "save", meta = (WorldContext = "WorldContext"))
	static void QuickSave(UObject* WorldContext);
	UFUNCTION(BlueprintCallable, Category = "save", meta = (WorldContext = "WorldContext"))
	static void QuickLoad(UObject* WorldContext);
	
	UFUNCTION(BlueprintCallable, Category = "save", meta = (WorldContext = "WorldContext"))
	static void LoadGameBySlot(UObject* WorldContext, USaveSlot* Slot);
	UFUNCTION(BlueprintCallable, Category = "save", meta = (WorldContext = "WorldContext"))
	static void DeleteGameBySlot(UObject* WorldContext, USaveSlot* Slot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "save", meta = (WorldContext = "WorldContext"))
	static bool HasSavedGame(UObject* WorldContext);	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "save", meta = (WorldContext = "WorldContext"))
	static TArray<USaveSlot*> GetAllSavedGame(UObject* WorldContext);
	
	
#pragma endregion

#pragma region Inventory
	/// Consumes Item if its consumable and applies the GameplayEffect accordingly
	/// @param ItemToConsume Item type to consume
	/// @param Amount How many you consume, negative number means all
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool ConsumeItem(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToConsume, const int32 Amount = 1, const int32 PlayerIndex = 0);
	
	/// Adds Item to the players inventory
	/// @param ItemToAdd Item type to add
	/// @param Amount How many you add, negative number means infinite
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool AddItemToPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToAdd, const int32 Amount = 1, const int32 PlayerIndex = 0);
	/// Gives you Infinite amount of the item to the player
	/// @param ItemToAdd Item type to add infinitely
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static void GrantInfiniteItemToPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToAdd, const int32 PlayerIndex = 0);
	/// Removes Item from the players inventory, shows if the remove was succesful
	/// @param ItemToRemove Item type to remove
	/// @param Amount How many you want to remove. If you want to subtract more than we have, it will not be removed. Negative number means infinite, it removes all instance
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool RemoveItemFromPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToRemove, const int32 Amount = 1, const int32 PlayerIndex = 0);
	/// Drops Item on the ground from the players inventory, shows if the remove was succesful
	/// @param ItemToDrop Item type to drop
	/// @param Amount How many you drop, If you want to subtract more than we have, it will not be removed. Negative number means infinite, it removes all instance
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool DropItemFromPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToDrop, const int32 Amount = 1, const int32 PlayerIndex = 0);
	///Wipes the whole player inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static void FlushPlayerInventory(const UObject* WorldContextObject, const int32 PlayerIndex = 0);

	/// Shows how many instance there is in the player inventory from the given item type
	/// @param ItemToCheck Item type to check, if its null we get all item types count
	/// @return the amount of the instances of the given item type
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static int32 GetNumberOfItemsInPlayerInventory(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToCheck, const int32 PlayerIndex = 0);
	/// Checks if Item type exist in the player Inventory
	/// @param ItemToCheck Item type to inspect
	/// @param OutCount How many instance you have from the given item type
	/// @return returns true if the item exists in the inventory
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool IsItemInPlayerInventory(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToCheck, int32& OutCount, const int32 PlayerIndex = 0);
	/// Gets all Item types which are present in the player inventory.
	/// @param OutNumberOfItemTypes how many types we can find
	/// @return The actual array of the itemtypes
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static TArray<UGD_Item_DataAsset*>
	GetAllItemTypesFromPlayerInventory(const UObject* WorldContextObject, int32& OutNumberOfItemTypes, const int32 PlayerIndex = 0);
	/// GetPlayerInventory map
	/// @return The whole inventory in Map form
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static TArray<FInventorySlot> GetPlayerInventory(const UObject* WorldContextObject, const int32 PlayerIndex = 0);
	
	/// Gets Player Inventory Component
	/// @return The inventory component of the player
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static UGD_InventoryComponent* GetPlayerInventoryComponent(const UObject* WorldContextObject, const int32 PlayerIndex = 0);
#pragma endregion
};
