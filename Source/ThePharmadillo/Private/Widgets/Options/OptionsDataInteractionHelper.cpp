// Gondos Daniel all rights reserved.


#include "Widgets/Options/OptionsDataInteractionHelper.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath) : CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedGameUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		OutStringValue);


	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InStringValue
	);
}
