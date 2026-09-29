// Gondos Daniel all rights reserved.


#include "Subsystems/FrontendAudioSubsystem.h"

#include "Components/AudioComponent.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"


UFrontendAudioSubsystem* UFrontendAudioSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendAudioSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UFrontendAudioSubsystem::InitializeAudioSettings()
{
	SetMasterVolume(UFrontendGameUserSettings::Get()->GetOverallVolume());
	SetMusicVolume(UFrontendGameUserSettings::Get()->GetMusicVolume());
	SetSFXVolume(UFrontendGameUserSettings::Get()->GetSFXVolume());
}

void UFrontendAudioSubsystem::SetMasterVolume(float InMasterVolume)
{
	if (UObject* LoadedSoundClass = GetDefault<UFrontendDeveloperSettings>()->MasterSoundClass.TryLoad())
		SetVolume(InMasterVolume, LoadedSoundClass);
}

void UFrontendAudioSubsystem::SetMusicVolume(float InMusicVolume)
{
	if (UObject* LoadedSoundClass = GetDefault<UFrontendDeveloperSettings>()->MusicSoundClass.TryLoad())
		SetVolume(InMusicVolume, LoadedSoundClass);
}

void UFrontendAudioSubsystem::SetSFXVolume(float InSFXVolume)
{
	if (UObject* LoadedSoundClass = GetDefault<UFrontendDeveloperSettings>()->SoundFXSoundClass.TryLoad())
		SetVolume(InSFXVolume, LoadedSoundClass);
}

void UFrontendAudioSubsystem::SetVolume(float InVolume, UObject* LoadedSoundClass)
{
	USoundClass* MusicSoundClass = nullptr;
	if (LoadedSoundClass)
	{
		MusicSoundClass = CastChecked<USoundClass>(LoadedSoundClass);
	}

	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = GetDefault<UFrontendDeveloperSettings>()->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}

	UGameplayStatics::SetSoundMixClassOverride(
		GEngine->GetCurrentPlayWorld(),
		DefaultSoundMix,
		MusicSoundClass,
		InVolume,
		1.f,
		0.f
	);

	UGameplayStatics::PushSoundMixModifier(GEngine->GetCurrentPlayWorld(), DefaultSoundMix);
}

void UFrontendAudioSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UAudioComponent* UFrontendAudioSubsystem::LazyPlayMusic(TSoftObjectPtr<USoundBase> MusicToPlaySoftPtr, bool ForceRestart, bool bIsUISound)
{
	return PlayMusic(MusicToPlaySoftPtr.LoadSynchronous(), ForceRestart, bIsUISound);
}

UAudioComponent* UFrontendAudioSubsystem::PlayMusic(USoundBase* MusicToPlay, bool ForceRestart, bool bIsUISound)
{
	if (!MusicToPlay) return nullptr;
	if (ActiveMusicComponent && ActiveMusicComponent->IsPlaying() && (MusicToPlay == ActiveMusic && !ForceRestart)) return nullptr;

	ActiveMusic = MusicToPlay;
	
	if (ActiveMusicComponent)
		ActiveMusicComponent->Stop();
	
	ActiveMusicComponent = UGameplayStatics::CreateSound2D(this, ActiveMusic, 1.0f, 1.0f, 0.0f, nullptr, true, false);
	ActiveMusicComponent->bIsUISound = bIsUISound;
	ActiveMusicComponent->Play();
	return ActiveMusicComponent;
}

void UFrontendAudioSubsystem::PauseMusic(const bool bPaused)
{
	if (ActiveMusicComponent && !ActiveMusicComponent->bIsUISound)
		ActiveMusicComponent->SetPaused(bPaused);
}

void UFrontendAudioSubsystem::FadeOutMusic(const float FadeTime)
{
	if (ActiveMusicComponent)
		ActiveMusicComponent->FadeOut(FadeTime, 0.f);
}
