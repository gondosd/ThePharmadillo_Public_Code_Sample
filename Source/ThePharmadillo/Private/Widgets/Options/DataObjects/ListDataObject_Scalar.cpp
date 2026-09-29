// Gondos Daniel all rights reserved.


#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString()));
	}
	return 0.f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue);

		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));
		NotifyListDataModified(this);
	}
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicSetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(CurrentValue, DefaultValue, 0.01f);
	}

	return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue() && DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
		NotifyListDataModified(this, EOptionsListDataModifiedReason::ResetToDefault);
		return true;
	}

	return false;
}

void UListDataObject_Scalar::OnEditDependencyModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifiedReason ModifiedReason)
{
	NotifyListDataModified(this, EOptionsListDataModifiedReason::DependencyModified);

	Super::OnEditDependencyModified(ModifiedDependencyData, ModifiedReason);
}

bool UListDataObject_Scalar::CanSetToForcedStringValue(const FString& InForcedValue) const
{
	return GetCurrentValue() != StringToFloat(InForcedValue);
}

void UListDataObject_Scalar::OnSetToForcedStringValue(const FString& InForcedValue)
{
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(InForcedValue);
		NotifyListDataModified(this, EOptionsListDataModifiedReason::DependencyModified);
	}
}

float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);

	return OutConvertedValue;
}
