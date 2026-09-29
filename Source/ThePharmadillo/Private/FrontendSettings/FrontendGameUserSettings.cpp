// Gondos Daniel all rights reserved.


#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendAudioSubsystem.h"
#include "Subsystems/FrontendHapticsSubsystem.h"

UFrontendGameUserSettings::UFrontendGameUserSettings() :
	HapticsIntensityMultiplier(1.f),
	bAllowUIControllerRumble(true),
	bAllowGameplayControllerRumble(true),
	OverallVolume(1.f),
	MusicVolume(1.f),
	SFXVolume(1.f),
	bAllowBackgroundAudio(false),
	bUseHDRAudioMode(false)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
}

void UFrontendGameUserSettings::SetHapticsIntensityMultiplier(const float InIntensityMultiplier)
{
	HapticsIntensityMultiplier = InIntensityMultiplier;
	OnHapticsChanged.Broadcast(InIntensityMultiplier);
}

void UFrontendGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
	UFrontendAudioSubsystem::SetMasterVolume(InVolume);
}

void UFrontendGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
	UFrontendAudioSubsystem::SetMusicVolume(InVolume);
}

void UFrontendGameUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;
	UFrontendAudioSubsystem::SetSFXVolume(InVolume);
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
	//TODO: implement the Audio code here
}

void UFrontendGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
	//TODO: implement the Audio code here
}

float UFrontendGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}
	return 0.f;
}

void UFrontendGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}
