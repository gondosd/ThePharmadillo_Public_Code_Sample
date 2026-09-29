// Gondos Daniel all rights reserved.


#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	//Notify the child list data to init itself
	InChildListData->InitDataObject();

	// Set the dhild list data's parent to this
	InChildListData->SetParentData(this);

	ChildListDataArray.Add(InChildListData);
}

UListDataObject_Collection* UListDataObject_Collection::FindChildListDataFromID(const FName& InDataID)
{
	const TArray<UListDataObject_Base*>& ChildListData = GetAllChildListData();
	
	// If there are no collections, the highest one should catch the List entries 
	if (ChildListData.Num() == 0)
		return this; //BUG: In theory this should work, but it crashes for some reason. Investigate!

	UListDataObject_Base* const* FoundCollection = ChildListData.FindByPredicate(
		[InDataID](const UListDataObject_Base* CurrentCollection)
		{
			return CurrentCollection->GetDataID() == InDataID;
		}
	);	
	
	if (!FoundCollection)
		return Cast<UListDataObject_Collection>(GetAllChildListData().Last());
	
	UListDataObject_Collection* CastedFoundCollection =Cast<UListDataObject_Collection>(*FoundCollection);
	
	//if it cant find any child, should return itself as fallback
	return CastedFoundCollection ? CastedFoundCollection : Cast<UListDataObject_Collection>(GetAllChildListData().Last());
}

TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
