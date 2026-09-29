// Gondos Daniel all rights reserved.


#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "FrontendDebugHelper.h"
#include "FrontendSettings/FrontendEnhancedInputUserSettings.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile,
                                                const FPlayerKeyMapping& InOwningPlayerKeyboardKeyMapping, const FPlayerKeyMapping& InOwningPlayerGamepadKeyMapping)
{
	CachedOwningInputUserSettings = CastChecked<UFrontendEnhancedInputUserSettings>(InOwningInputUserSettings);
	CachedOwningKeyProfile = InKeyProfile;
	CachedOwningMappingName = InOwningPlayerGamepadKeyMapping.GetMappingName(); //this is the same in both Mapping data
	
	CachedOwningMappableKeyboardKeySlot = InOwningPlayerKeyboardKeyMapping.GetSlot(); // should be always first
	CachedOwningMappableGamepadKeySlot = InOwningPlayerGamepadKeyMapping.GetSlot(); // should be always second
	
	CommonInputSubsystem = UCommonInputSubsystem::Get(CachedOwningInputUserSettings->GetLocalPlayer());
	check(CommonInputSubsystem);
}

FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey(ECommonInputType InputType) const
{
	check(CachedOwningInputUserSettings);

	FSlateBrush FoundBrush;

	

	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush,
		GetOwningKeyMapping()->GetCurrentKey(),
		InputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	if (!bHasFoundBrush)
	{
		Debug::Print(TEXT("Unable to find an icon for the key") +
			GetOwningKeyMapping()->GetCurrentKey().GetDisplayName().ToString() +
			TEXT(" Empty brush was applied"));
	}

	return FoundBrush;
}

void UListDataObject_KeyRemap::BindNewInputKey(const FKey& InNewKey, ECommonInputType CurrentInputType)
{
	check(CachedOwningInputUserSettings);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = GetSlotFromCurrentInputType();
	
	KeyArgs.NewKey = InNewKey;
	
	FGameplayTagContainer Container;
	CachedOwningInputUserSettings->MapPlayerKey(KeyArgs, Container);
	CachedOwningInputUserSettings->SaveSettings();
	
	NotifyListDataModified(this);
}

bool UListDataObject_KeyRemap::HasDefaultValue() const
{
	return GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UListDataObject_KeyRemap::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && GetOwningKeyMapping()->IsCustomized();
}

bool UListDataObject_KeyRemap::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		check(CachedOwningInputUserSettings);
		
		FMapPlayerKeyArgs KeyArgs;
		KeyArgs.MappingName = CachedOwningMappingName;
		KeyArgs.Slot = GetSlotFromCurrentInputType();
	
		FGameplayTagContainer Container;
		CachedOwningInputUserSettings->ResetAllPlayerKeysInRowForSlot(KeyArgs, Container);
		
		CachedOwningInputUserSettings->SaveSettings();
		
		NotifyListDataModified(this, EOptionsListDataModifiedReason::ResetToDefault);
		
		return true;
	}
	
	return false;
}

EPlayerMappableKeySlot UListDataObject_KeyRemap::GetSlotFromCurrentInputType() const
{
	switch (CommonInputSubsystem->GetCurrentInputType())
	{
	case ECommonInputType::MouseAndKeyboard:
		return CachedOwningMappableKeyboardKeySlot;
	case ECommonInputType::Gamepad:
		return CachedOwningMappableGamepadKeySlot;
	default: 
		UE_LOG(LogTemp, Error, TEXT("Input type is not Mouse and keyboard nor gamepad. please prepare this function for those possibilities %s"), *FString(__FUNCTION__));
	}
	return EPlayerMappableKeySlot::Unspecified;
}

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping() const
{
	check(CachedOwningKeyProfile);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = GetSlotFromCurrentInputType();

	return CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}
