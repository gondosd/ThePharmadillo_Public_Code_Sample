// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendAudioSubsystem.generated.h"

class UFrontendDeveloperSettings;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UFrontendAudioSubsystem* Get(const UObject* WorldContextObject);
	
	static void InitializeAudioSettings();
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	UAudioComponent* LazyPlayMusic(TSoftObjectPtr<USoundBase> MusicToPlaySoftPtr, bool ForceRestart, bool bIsUISound = true);
	
	UFUNCTION(BlueprintCallable)
	UAudioComponent* PlayMusic(USoundBase* MusicToPlay, bool ForceRestart, bool bIsUISound = true);
	
	UFUNCTION(BlueprintCallable)
	void PauseMusic(const bool bPaused);
	
	UFUNCTION(BlueprintCallable)
	void FadeOutMusic(const float FadeTime = 1.f);
	
	static void SetMasterVolume(float InMasterVolume);
	static void SetMusicVolume(float InMusicVolume);
	static void SetSFXVolume(float InSFXVolume);
	
	static void SetVolume(float InVolume, UObject* LoadedSoundClass);
	
private:
	
	static const UFrontendDeveloperSettings* CachedFrontendDeveloperSettings;
	
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> ActiveMusicComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundBase> ActiveMusic;

	
};
