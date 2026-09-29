#pragma once
#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionsDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(const TFunction<bool()>& InEditConditionFunc) { EditConditionFunc = InEditConditionFunc; }
	bool IsValid() const { return EditConditionFunc != nullptr; }

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}
		return true;
	}

	FString GetDisabledRichReason() const { return DisabledRichReason; }

	void SetDisabledRichReason(const FString& InDisabledRichReason) { DisabledRichReason = InDisabledRichReason; }

	bool HasForcedStringValue() const { return DisabledForcedStringValue.IsSet(); }
	FString GetDisabledForcedStringValue() const { return DisabledForcedStringValue.GetValue(); }
	void SetDisabledForcedStringValue(const FString& InForcedValue) { DisabledForcedStringValue = InForcedValue; }

private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};
