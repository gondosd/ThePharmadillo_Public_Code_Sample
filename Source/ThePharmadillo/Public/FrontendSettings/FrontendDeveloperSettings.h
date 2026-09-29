// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_ActivatableWidgetBase;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Frontend UI Settings"))
class THEPHARMADILLO_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "Frontend.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableWidgetBase>> FrontendWidgetMap;
	
	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta = (ForceInlineRow, Categories = "Frontend.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;
	
	//AUDIO
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;
     
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;
     
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath SoundFXSoundClass;
     
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;
	
	UPROPERTY(Config, EditAnywhere, Category = "Audio")
	TSoftObjectPtr<USoundBase> InventoryFullWarningSound;
	
	//HAPTICS
	UPROPERTY(Config, EditAnywhere, Category = "Haptics")
	TSoftObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect_Default_Weak;
	
	UPROPERTY(Config, EditAnywhere, Category = "Haptics")
	TSoftObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect_Default_Strong;
	
	//Actor classes
	UPROPERTY(Config, EditAnywhere, Category = "Global Actors")
	TSoftClassPtr<AActor> CollectableItem;
	
};
