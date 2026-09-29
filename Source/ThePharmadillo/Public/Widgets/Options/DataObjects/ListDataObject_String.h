// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FrontendDebugHelper.h"
#include "ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	void AdvanceToNextOption();
	void BackToPreviousOption();

	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);

	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

protected:
	virtual void OnDataObjectInitialized() override;
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) override;

	FString CurrentStringValue;
	FText CurrentDisplayText;

	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE const FText& GetCurrentDisplayText() const { return CurrentDisplayText; }
};

UCLASS()
class THEPHARMADILLO_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

	bool GetCurrentValueAsBool() const { return CurrentStringValue == TrueString; }

protected:
	virtual void OnDataObjectInitialized() override;

private:
	void TryInitBoolValues();

	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

UCLASS()
class THEPHARMADILLO_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:
	template <typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}

	template <typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return static_cast<EnumType>(StaticEnumOption->GetValueByNameString(CurrentStringValue, EGetByNameFlags::CaseSensitive));
	}

	template <typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}
};

UCLASS()
class THEPHARMADILLO_API UListDataObject_StringInteger : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);

protected:
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependencyModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifiedReason ModifiedReason) override;
};
