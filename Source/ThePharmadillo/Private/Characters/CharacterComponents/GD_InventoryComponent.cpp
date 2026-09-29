// Gondos Daniel all rights reserved.


#include "Characters/CharacterComponents/GD_InventoryComponent.h"

#include "FrontendDebugHelper.h"
#include "Actors/GD_CollectableActor_Base.h"
#include "DataAssets/GD_Item_DataAsset.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "GameFramework/Character.h"
#include "GameplayEffects/GD_GameplayEffect_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/FrontendUISubsystem.h"


UGD_InventoryComponent::UGD_InventoryComponent()
{
	CachedFrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
}

void UGD_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Inventory.Num() != InventorySize)
	{
		Inventory.SetNum(InventorySize);
	}
}

bool UGD_InventoryComponent::ConsumeItem(UGD_Item_DataAsset* ItemToConsume, const int32 Amount)
{
	if (!ItemToConsume) return false; // there is nothing to consume

	if (auto GameplayEffect = ItemToConsume->GameplayEffect.LoadSynchronous())
	{
		if (RemoveItem(ItemToConsume, Amount))
		{
			UGD_GameplayEffect_Base* EffectInstance = NewObject<UGD_GameplayEffect_Base>(this, GameplayEffect);
			EffectInstance->Activate(this);
			
			PlayAddSoundAsync(ItemToConsume->InventoryConsumeSound);
			
			return true;
		}
	}
	else
	{
		Debug::Print("Item has no GameplayEffect, so it means its not consumable!");
	}
	return false;
}

bool UGD_InventoryComponent::AddItem(UGD_Item_DataAsset* ItemToAdd, const int32 Amount)
{
	if (!ItemToAdd)
	{
		Debug::Print("ItemToAdd is empty!");
		return false;
	}
	
	PlayAddSoundAsync(ItemToAdd->InventoryAddSound);

	if (FInventorySlot* FoundSlot = Inventory.FindByKey(ItemToAdd))
	{
		if (Amount < 0) //Negative number counts as infinite
			FoundSlot->Quantity = -1;

		if (FoundSlot->Quantity >= 0)
			FoundSlot->Quantity += Amount;
	}
	else if (Amount != 0)
	{
		const int32 EmptyIndex = Inventory.IndexOfByPredicate([](const FInventorySlot& Slot) { return Slot.IsEmpty(); });
		if (!Inventory.IsValidIndex(EmptyIndex))
		{
			InventoryIsFullMessaging(ItemToAdd, Amount);
			return false;
		}

		Inventory[EmptyIndex] = FInventorySlot(ItemToAdd, Amount);
	}
	
	OnInventoryUpdated.Broadcast(this, ItemToAdd);
	return true;
}

void UGD_InventoryComponent::GrantInfiniteItem(UGD_Item_DataAsset* ItemToAdd)
{
	if (!ItemToAdd)
	{
		Debug::Print("ItemToAdd is empty!");
		return;
	}
	AddItem(ItemToAdd, -1);
}

bool UGD_InventoryComponent::RemoveItem(UGD_Item_DataAsset* ItemToRemove, const int32 Amount)
{
	if (!ItemToRemove)
	{
		Debug::Print("ItemToRemove is empty!");
		return false;
	}

	const int32 FoundIndex = Inventory.IndexOfByKey(ItemToRemove);
	if (!Inventory.IsValidIndex(FoundIndex))
	{
		Debug::Print("Item is not in the Inventory");
		return false;
	}

	int32& CurrentQuantity = Inventory[FoundIndex].Quantity;

	if ((CurrentQuantity > 0) && (Amount > 0) && (CurrentQuantity > Amount))
	//currently positive AND we reduct a noninfinite num AND after reduction it wont be negative
	{
		CurrentQuantity -= Amount;
	}
	else if ((CurrentQuantity >= Amount) || (Amount < 0)) //if reduction would resolve a negative number or zero OR infinite reduction
	{
		Inventory[FoundIndex] = FInventorySlot();
	}
	else
	{
		Debug::Print("You dont have enough item in your inventory for this transaction");
		return false;
	}

	OnInventoryUpdated.Broadcast(this, ItemToRemove);
	return true;
}

bool UGD_InventoryComponent::DropItem(UGD_Item_DataAsset* ItemToDrop, const int32 Amount, bool bForceDropItem)
{
	int32 AmountToSpawn = Amount;
	if (Amount < 0) //if presented with a negative number we drop all items from the inventory
		AmountToSpawn = GetNumberOfItemsInInventory(ItemToDrop);

	if (!bForceDropItem && !RemoveItem(ItemToDrop, Amount))
		return false;

	PlayAddSoundAsync(ItemToDrop->InventoryAddSound);
	
	if (UClass* ActorClass = CachedFrontendDeveloperSettings->CollectableItem.LoadSynchronous())
	{
		for (int i = 0; i < AmountToSpawn; ++i)
		{
			if (AGD_CollectableActor_Base* SpawnedItem = GetWorld()->SpawnActorDeferred<AGD_CollectableActor_Base>(
				ActorClass, GetOwner()->GetTransform(), GetOwner()))
			{
				SpawnedItem->ItemType = ItemToDrop;
				SpawnedItem->SetActorTransform(UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorTransform());
				SpawnedItem->SetStaticMesh();
				SpawnedItem->FinishSpawning(GetOwner()->GetTransform());
			}
		}
	}
	
	return true;
}

void UGD_InventoryComponent::FlushInventory()
{
	for (FInventorySlot& Slot : Inventory)
		Slot = FInventorySlot();

	OnInventoryUpdated.Broadcast(this, nullptr);
}

bool UGD_InventoryComponent::TransferSlots(const int32 SourceIndex, UGD_InventoryComponent* TargetInventoryComponent, const int32 TargetIndex)
{
	if (TargetIndex < 0 ) return AutoMoveSlot(SourceIndex, TargetInventoryComponent); //automatically move to the first available slot or merge
	
	if (!Inventory.IsValidIndex(SourceIndex) || !TargetInventoryComponent || !TargetInventoryComponent->GetInventory().IsValidIndex(TargetIndex)) return false;
	
	FInventorySlot& SourceItemSlot = Inventory[SourceIndex];
	FInventorySlot& TargetItemSlot = TargetInventoryComponent->Inventory[TargetIndex];
	
	if (!SourceItemSlot.IsEmpty())
		PlayAddSoundAsync(SourceItemSlot.ItemData->InventoryAddSound);
	if (!TargetItemSlot.IsEmpty())
		PlayAddSoundAsync(TargetItemSlot.ItemData->InventoryAddSound);


	if (SourceItemSlot == TargetItemSlot)
	{
		if (FInventorySlot* FoundSlot = TargetInventoryComponent->Inventory.FindByKey(TargetItemSlot))
		{
			if (SourceItemSlot.Quantity < 0) //Negative number counts as infinite
				FoundSlot->Quantity = -1;

			if (FoundSlot->Quantity >= 0)
				FoundSlot->Quantity += SourceItemSlot.Quantity;
			
			SourceItemSlot = FInventorySlot();  //removing the item from the source, to avoid duplications
		}
	}
	else
	{
		Exchange(SourceItemSlot, TargetItemSlot);
	}
	
	if (TargetInventoryComponent != this)
		TargetInventoryComponent->OnInventoryUpdated.Broadcast(this, nullptr);
	OnInventoryUpdated.Broadcast(this, nullptr);

	return true;
}

bool UGD_InventoryComponent::AutoMoveSlot(const int32 SourceIndex, UGD_InventoryComponent* TargetInventoryComponent)
{
	if (!TargetInventoryComponent || !Inventory.IsValidIndex(SourceIndex)) return false;

	FInventorySlot& SourceItemSlot = Inventory[SourceIndex];

	if (!SourceItemSlot.IsEmpty())
		PlayAddSoundAsync(SourceItemSlot.ItemData->InventoryAddSound);
	
	if (FInventorySlot* FoundSlot = TargetInventoryComponent->Inventory.FindByKey(SourceItemSlot.ItemData))
	{
		if (SourceItemSlot.Quantity < 0) //Negative number counts as infinite
			FoundSlot->Quantity = -1;

		if (FoundSlot->Quantity >= 0)
			FoundSlot->Quantity += SourceItemSlot.Quantity;

		SourceItemSlot = FInventorySlot(); //removing the item from the source, to avoid duplications
	}
	else
	{
		// No existing stack: drop into the target's first empty slot, if it has one.
		const int32 EmptyIndex = TargetInventoryComponent->Inventory.IndexOfByPredicate([](const FInventorySlot& Slot) { return Slot.IsEmpty(); });
		
		if (!TargetInventoryComponent->Inventory.IsValidIndex(EmptyIndex)) return false; //target inventory is full

		Exchange(SourceItemSlot, TargetInventoryComponent->Inventory[EmptyIndex]);
	}
	
	
	if (TargetInventoryComponent != this)
		TargetInventoryComponent->OnInventoryUpdated.Broadcast(this, nullptr);
	OnInventoryUpdated.Broadcast(this, nullptr);
	
	return true;
}

int32 UGD_InventoryComponent::GetNumberOfItemsInInventory(UGD_Item_DataAsset* ItemToCheck) const
{
	if (ItemToCheck)
	{
		const FInventorySlot* FoundSlot = Inventory.FindByKey(ItemToCheck);
		return FoundSlot ? FoundSlot->Quantity : 0;
	}

	int32 UsedSlots = 0;
	for (const FInventorySlot& Slot : Inventory)
	{
		if (!Slot.IsEmpty())
			++UsedSlots;
	}
	return UsedSlots; // if Item to check is null, we get all item num
}

bool UGD_InventoryComponent::IsItemInInventory(UGD_Item_DataAsset* ItemToCheck, int32& OutCount) const
{
	if (!ItemToCheck)
	{
		Debug::Print("ItemToCheck is empty!");
		return false;
	}

	OutCount = GetNumberOfItemsInInventory(ItemToCheck);
	return OutCount != 0;
}

TArray<UGD_Item_DataAsset*> UGD_InventoryComponent::GetAllItemTypesFromInventory(int32& OutNumberOfItemTypes) const
{
	TArray<UGD_Item_DataAsset*> Items;

	for (const FInventorySlot& Slot : Inventory)
	{
		if (Slot.ItemData)
			Items.AddUnique(Slot.ItemData);
	}

	OutNumberOfItemTypes = Items.Num();
	return Items;
}

void UGD_InventoryComponent::AddInventorySlot()
{
	Inventory.AddDefaulted();
	InventorySize = Inventory.Num();
	OnInventoryUpdated.Broadcast(this, nullptr);
}

void UGD_InventoryComponent::InventoryIsFullMessaging(UGD_Item_DataAsset* ItemToAdd, const int32 Amount)
{
	Debug::Print("This inventory is full!");

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsynch(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("Inventory full!")),
		FText::FromString(TEXT("Can't add ") + ItemToAdd->Name.ToString() + TEXT(" to the inventory because it's full")),
		[this, ItemToAdd, Amount](EConfirmScreenButtonType ClickedButtonType)
		{
			DropItem(ItemToAdd, Amount, true);
		}
	);

	UGameplayStatics::PlaySound2D(GetWorld(), CachedFrontendDeveloperSettings->InventoryFullWarningSound.LoadSynchronous());
}

void UGD_InventoryComponent::PlayAddSoundAsync(const TSoftObjectPtr<USoundBase> Sound) const
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound.Get() ? Sound.Get() : Sound.LoadSynchronous());
}
