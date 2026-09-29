// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

/**
 * 
 */
UCLASS(Abstract)
class THEPHARMADILLO_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) {DefaultStringValue = InDefaultValue;}
	virtual bool HasDefaultValue() const override {return DefaultStringValue.IsSet();}

protected:
	FString GetDefaultValueAsString() const{return DefaultStringValue.IsSet() ? DefaultStringValue.GetValue() : FString();}
	
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
};
