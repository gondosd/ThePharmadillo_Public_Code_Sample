// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Base;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	//Gets called by options screen right after the object of type UOptionsDataRegistry is created
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }
	
	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;
	
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlsCollectionTab();
	void InitKeyBindingsCollectionTab(ULocalPlayer* InOwningLocalPlayer);
	void InitAccessibilityCollectionTab();

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};
