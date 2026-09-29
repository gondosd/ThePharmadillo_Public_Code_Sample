// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "FrontendHapticsSubsystem.generated.h"

class UFrontendDeveloperSettings;
class APlayerController;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendHapticsSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendHapticsSubsystem* Get(const ULocalPlayer* LocalPlayer);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable, Category = "FrontendHaptics")
	void PlayForceFeedbackEffect (UForceFeedbackEffect* ForceFeedbackEffect, const ERumbleType RumbleType = ERumbleType::Gameplay);
	
	UFUNCTION(BlueprintCallable, Category = "FrontendHaptics")
	void PlayDefaultWeakFeedbackEffect (const ERumbleType RumbleType = ERumbleType::Gameplay);
	
	UFUNCTION(BlueprintCallable, Category = "FrontendHaptics")
	void PlayDefaultStrongFeedbackEffect (const ERumbleType RumbleType = ERumbleType::Gameplay);
	
	void ApplyHapticsIntensityMultiplier(float InValue) const;
	
private:
	static const UFrontendDeveloperSettings* CachedFrontendDeveloperSettings;
	
	UPROPERTY(Transient)
	TObjectPtr<UFrontendGameUserSettings> CachedUserSettings;
};
