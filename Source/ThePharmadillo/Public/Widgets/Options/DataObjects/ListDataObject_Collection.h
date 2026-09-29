// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void AddChildListData(UListDataObject_Base* InChildListData);
	
	UListDataObject_Collection* FindChildListDataFromID(const FName& InDataID);
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;

private:
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};
