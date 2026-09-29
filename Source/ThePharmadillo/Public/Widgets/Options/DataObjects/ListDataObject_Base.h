// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/Object.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "ListDataObject_Base.generated.h"

//This is a macro for setters\getters
#define LIST_DATA_ACCESSOR(DataType, PropertyName)\
	FORCEINLINE DataType Get##PropertyName() const {return PropertyName;}\
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName;}

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsListDataModifiedReason);

UCLASS(Abstract)
class THEPHARMADILLO_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;

#pragma region SettersGetters
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)
	
	void SetShouldApplyChangeImmediately(bool bShouldApplyRightAway) {bShouldApplyChangeImmediately = bShouldApplyRightAway;};
#pragma endregion

	void InitDataObject();

	//Empty in the base class. Child class ListDataObject_Collection should override it. The function should return all the child data a tab has
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	virtual bool HasAnyChildListData() const { return false; }
	
	//The Child Class should override them to provide implementations for resetting the data
	virtual bool HasDefaultValue()const {return false;}
	virtual bool CanResetBackToDefaultValue()const {return false;}
	virtual bool TryResetBackToDefaultValue() {return false;}
	
	// Gets called from OptionsDataRegister for adding in edit conditions for the constructed list data objects
	void AddEditCondition ( const FOptionsDataEditConditionsDescriptor& InEditCondition);

	// Gets called from OptionsDataRegister to add in dependency data
	void AddEditDependencyData(UListDataObject_Base* InDependencyData);
	
	bool IsDataCurrentlyEditable();
protected:
	// Empty in base class. The child classes should override it to handle the necessary initialization  
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifiedReason ModifiedReason = EOptionsListDataModifiedReason::DirectlyModified);

	//The child class should override this to allow the value to be  set to the forced string value
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const { return false; }
	//The child class should override this to specify how to set the current value to the forced value
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}
	
	//This function will be called when the value of the dependency data has changed. the child class can override this function to handle the custom logic needed. super call is expected
	virtual void OnEditDependencyModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifiedReason ModifiedReason);
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	bool bShouldApplyChangeImmediately = false;


	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionsDescriptor> EditConditionDescArray;
};
