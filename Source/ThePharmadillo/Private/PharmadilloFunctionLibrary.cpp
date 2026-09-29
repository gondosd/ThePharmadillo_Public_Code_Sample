// Gondos Daniel all rights reserved.


#include "PharmadilloFunctionLibrary.h"

#include "SaveManager.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "FrameworkClasses/GD_PlayerStateBase.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/FrontendLoadingScreenSubsystem.h"

const AGD_PlayerStateBase* UPharmadilloFunctionLibrary::GetGD_PlayerState(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	const APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex);
	check(PC);
	const AGD_PlayerStateBase* PS = PC->GetPlayerState<AGD_PlayerStateBase>();
	check(PS);
	return PS;
}

USaveManager* UPharmadilloFunctionLibrary::GetSaveSubsystem(const UObject* WorldContextObject)
{
	return USaveManager::Get(WorldContextObject);
}

FString UPharmadilloFunctionLibrary::GetCurrentLevelNameWithoutPersistentPrefix(const UObject* WorldContextObject)
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(WorldContextObject);
	CurrentLevelName.RemoveFromStart(TEXT("Persistent_"));
	return CurrentLevelName;
}

TSoftClassPtr<UWidget_ActivatableWidgetBase> UPharmadilloFunctionLibrary::GetFrontendSoftWidgetClassByTag(
	UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag),
	       TEXT("Cound not find the corresponding widget under the tag %s"), *InWidgetTag.ToString());

	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UPharmadilloFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->OptionsScreenSoftImageMap.Contains(InImageTag),
	       TEXT("Cound not find the corresponding image under the tag %s"), *InImageTag.ToString());

	return FrontendDeveloperSettings->OptionsScreenSoftImageMap.FindRef(InImageTag);
}

bool UPharmadilloFunctionLibrary::FileSaveString(FString StringToSave, FString FilePath)
{
	return FFileHelper::SaveStringToFile(StringToSave, *(FPaths::ProjectContentDir() + FilePath));
}

bool UPharmadilloFunctionLibrary::FileLoadString(FString FilePath, FString& OutString)
{
	return FFileHelper::LoadFileToString(OutString, *(FPaths::ProjectContentDir() + FilePath));
}

void UPharmadilloFunctionLibrary::QuickSave(UObject* WorldContext)
{
	auto Slotname = GetSaveSubsystem(WorldContext)->GetActiveSlot()->Name;
	
	if (Slotname == FName("Default")) //if its not a loadgame, we need a new slot
	{
		GetSaveSubsystem(WorldContext)->SaveSlot(FName(FGuid::NewGuid().ToString()), true, true);
	}
	else
	{
		GetSaveSubsystem(WorldContext)->SaveActiveSlot(true);
	}
}

void UPharmadilloFunctionLibrary::QuickLoad(UObject* WorldContext)
{
	LoadGameBySlot(WorldContext, GetAllSavedGame(WorldContext)[0]);	
}

void UPharmadilloFunctionLibrary::LoadGameBySlot(UObject* WorldContext, USaveSlot* Slot)
{
	UFrontendLoadingScreenSubsystem::Get(WorldContext)->CacheLoadingFile(Slot->Name); //the rest of this hack will be executed in the LoadingScreenSubsystem
	GetSaveSubsystem(WorldContext)->SetActiveSlot(Slot);
	UGameplayStatics::OpenLevel(WorldContext, Slot->Map, true);
}

void UPharmadilloFunctionLibrary::DeleteGameBySlot(UObject* WorldContext, USaveSlot* Slot)
{
	GetSaveSubsystem(WorldContext)->DeleteSlot(Slot);
}

bool UPharmadilloFunctionLibrary::HasSavedGame(UObject* WorldContext)
{
	return GetSaveSubsystem(WorldContext)->PreloadAllSlotsSync().Num() > 0;
}

TArray<USaveSlot*> UPharmadilloFunctionLibrary::GetAllSavedGame(UObject* WorldContext)
{
	return GetSaveSubsystem(WorldContext)->PreloadAllSlotsSync(true);
}

bool UPharmadilloFunctionLibrary::ConsumeItem(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToConsume, const int32 Amount, const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->ConsumeItem(ItemToConsume, Amount);
}

bool UPharmadilloFunctionLibrary::AddItemToPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToAdd, const int32 Amount, const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->AddItem(ItemToAdd, Amount);
}

void UPharmadilloFunctionLibrary::GrantInfiniteItemToPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToAdd, const int32 PlayerIndex)
{
	GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->GrantInfiniteItem(ItemToAdd);
}

bool UPharmadilloFunctionLibrary::RemoveItemFromPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToRemove, const int32 Amount,
                                                       const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->RemoveItem(ItemToRemove, Amount);
}

bool UPharmadilloFunctionLibrary::DropItemFromPlayer(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToDrop, const int32 Amount,
	const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->DropItem(ItemToDrop, Amount);
}

void UPharmadilloFunctionLibrary::FlushPlayerInventory(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->FlushInventory();
}

int32 UPharmadilloFunctionLibrary::GetNumberOfItemsInPlayerInventory(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToCheck,
                                                                     const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->GetNumberOfItemsInInventory(ItemToCheck);
}

bool UPharmadilloFunctionLibrary::IsItemInPlayerInventory(const UObject* WorldContextObject, UGD_Item_DataAsset* ItemToCheck, int32& OutCount,
                                                          const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->IsItemInInventory(ItemToCheck, OutCount);
}

TArray<UGD_Item_DataAsset*> UPharmadilloFunctionLibrary::GetAllItemTypesFromPlayerInventory(const UObject* WorldContextObject, int32& OutNumberOfItemTypes,
                                                                                            const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->GetAllItemTypesFromInventory(OutNumberOfItemTypes);
}

TArray<FInventorySlot> UPharmadilloFunctionLibrary::GetPlayerInventory(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return GetPlayerInventoryComponent(WorldContextObject, PlayerIndex)->GetInventory();
}

UGD_InventoryComponent* UPharmadilloFunctionLibrary::GetPlayerInventoryComponent(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return GetGD_PlayerState(WorldContextObject, PlayerIndex)->GetInventoryComponent();
}
