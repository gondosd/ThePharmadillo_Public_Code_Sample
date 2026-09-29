// Gondos Daniel all rights reserved.


#include "Subsystems/FrontendHapticsSubsystem.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "GameFramework/PlayerController.h"

const UFrontendDeveloperSettings* UFrontendHapticsSubsystem::CachedFrontendDeveloperSettings = nullptr;

UFrontendHapticsSubsystem* UFrontendHapticsSubsystem::Get(const ULocalPlayer* LocalPlayer)
{
	return LocalPlayer ? LocalPlayer->GetSubsystem<UFrontendHapticsSubsystem>() : nullptr;
}

void UFrontendHapticsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CachedUserSettings = Cast<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	if (CachedUserSettings)
	{
		// Init for feedback intensity
		ApplyHapticsIntensityMultiplier(CachedUserSettings->GetHapticsIntensityMultiplier());

		CachedUserSettings->OnHapticsChanged.AddUObject(this, &UFrontendHapticsSubsystem::ApplyHapticsIntensityMultiplier);
	}
}

void UFrontendHapticsSubsystem::PlayForceFeedbackEffect(UForceFeedbackEffect* ForceFeedbackEffect, const ERumbleType RumbleType)
{
	if (!CachedUserSettings->GetAllowUIControllerRumble() && (RumbleType == ERumbleType::Menu))
		return; //This is turned off
	if (!CachedUserSettings->GetAllowGameplayControllerRumble() && (RumbleType == ERumbleType::Gameplay))
		return; //This is turned off

	if (APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		FForceFeedbackParameters Params;
		Params.bPlayWhilePaused = true;
		PC->ClientPlayForceFeedback(ForceFeedbackEffect, Params);
	}
}

void UFrontendHapticsSubsystem::PlayDefaultWeakFeedbackEffect(const ERumbleType RumbleType)
{
	if (!CachedFrontendDeveloperSettings)
		CachedFrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	PlayForceFeedbackEffect(CachedFrontendDeveloperSettings->ForceFeedbackEffect_Default_Weak.LoadSynchronous(), RumbleType);
}

void UFrontendHapticsSubsystem::PlayDefaultStrongFeedbackEffect(const ERumbleType RumbleType)
{
	if (!CachedFrontendDeveloperSettings)
		CachedFrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	PlayForceFeedbackEffect(CachedFrontendDeveloperSettings->ForceFeedbackEffect_Default_Strong.LoadSynchronous(), RumbleType);
}

void UFrontendHapticsSubsystem::ApplyHapticsIntensityMultiplier(const float InValue) const
{
	if (APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		PC->ForceFeedbackScale = InValue * InValue; //This is necessary so the value changes are a bit more noticeable
	}
}
