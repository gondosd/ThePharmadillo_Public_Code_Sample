// Gondos Daniel all rights reserved.


#include "FrontendSettings/FrontendEnhancedInputUserSettings.h"

#include "EnhancedInputModule.h"

void UFrontendEnhancedInputUserSettings::ResetAllPlayerKeysInRowForSlot(const FMapPlayerKeyArgs& InArgs,
                                                                 FGameplayTagContainer& FailureReason)
{
	// Get the key profile that was specified
	UFrontendEnhancedPlayerMappableKeyProfile* KeyProfile = CastChecked<UFrontendEnhancedPlayerMappableKeyProfile>(
		InArgs.ProfileIdString.IsEmpty() ? GetActiveKeyProfile(): GetKeyProfileWithId(InArgs.ProfileIdString));
	
	if (FKeyMappingRow* ExistingMappings = KeyProfile->PlayerMappedKeys.Find(InArgs.MappingName))
	{
		for (FPlayerKeyMapping& Mapping : ExistingMappings->Mappings)
		{
			if (Mapping.GetSlot() != InArgs.Slot)
				continue;
				
			// Then set the player mapped key
			Mapping.ResetToDefault();

			// The settings have only changed if the mapping is dirty now
			if (Mapping.IsDirty())
			{
				OnKeyMappingUpdated(&Mapping, InArgs, true);
				OnSettingsChanged.Broadcast(this);
			}
			UE_LOG(LogEnhancedInput, Verbose, TEXT("[UEnhancedInputUserSettings::MapPlayerKey] Reset keymapping to default: '%s'"), *Mapping.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Mapping found! "))
	}
}
