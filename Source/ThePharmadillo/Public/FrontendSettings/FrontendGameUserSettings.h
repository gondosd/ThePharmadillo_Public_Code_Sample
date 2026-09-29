// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHapticsIntensityChanged, float /*NewIntensity*/);

class UFrontendAudioSubsystem;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFrontendGameUserSettings();
	static UFrontendGameUserSettings* Get();

	//******** Gameplay Collection Tab **********/////
	UFUNCTION()
	FORCEINLINE FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	FORCEINLINE void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; };

	UFUNCTION()
	FORCEINLINE float GetHapticsIntensityMultiplier() const { return HapticsIntensityMultiplier; }

	UFUNCTION()
	void SetHapticsIntensityMultiplier(const float InIntensityMultiplier);
	FOnHapticsIntensityChanged OnHapticsChanged;

	UFUNCTION()
	FORCEINLINE bool GetAllowUIControllerRumble() const { return bAllowUIControllerRumble; }
	UFUNCTION()
	FORCEINLINE void SetAllowUIControllerRumble(const bool bInAllowRumble) { bAllowUIControllerRumble = bInAllowRumble; }

	UFUNCTION()
	FORCEINLINE bool GetAllowGameplayControllerRumble() const { return bAllowGameplayControllerRumble; }
	UFUNCTION()
	FORCEINLINE void SetAllowGameplayControllerRumble(const bool bInAllowRumble) { bAllowGameplayControllerRumble = bInAllowRumble; }

	UFUNCTION()
	FORCEINLINE bool GetInvertLook() const { return bInvertLook; }
	UFUNCTION()
	FORCEINLINE void SetInvertLook(const bool bInInvertLook) { bInvertLook = bInInvertLook; }
	UFUNCTION()
	FORCEINLINE float GetLookSensitivity() const { return LookSensitivity; }
	UFUNCTION()
	FORCEINLINE void SetLookSensitivity(const float InLookSensitivity) { LookSensitivity = InLookSensitivity; }
	
	UFUNCTION()
	FORCEINLINE bool GetAllowCameraFollowRollMovement() const { return AllowCameraFollowRollMovement; }
	UFUNCTION()
	FORCEINLINE void SetAllowCameraFollowMovement(const bool InAllowCameraFollowMovement) { AllowCameraFollowRollMovement = InAllowCameraFollowMovement; }

	UFUNCTION()
	FORCEINLINE bool GetAllowToggleSprint() const { return AllowToggleSprint; }
	UFUNCTION()
	FORCEINLINE void SetAllowToggleSprint(const bool InAllowToggleSprint) { AllowToggleSprint = InAllowToggleSprint; }

	//******** Gameplay Collection Tab **********/////


	//******** Audio Collection Tab **********/////
	UFUNCTION()
	FORCEINLINE float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float InVolume);
	UFUNCTION()
	FORCEINLINE float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float InVolume);
	UFUNCTION()
	FORCEINLINE float GetSFXVolume() const { return SFXVolume; }

	UFUNCTION()
	void SetSFXVolume(float InVolume);

	UFUNCTION()
	FORCEINLINE bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);
	UFUNCTION()
	FORCEINLINE bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);

	//******** Audio Collection Tab **********/////

	//******** Video Collection Tab **********/////
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	//******** Video Collection Tab **********/////

private:
	//******** Gameplay Collection Tab **********/////
	UPROPERTY(config)
	FString CurrentGameDifficulty;

	UPROPERTY(config)
	float HapticsIntensityMultiplier;

	UPROPERTY(config)
	bool bAllowUIControllerRumble;

	UPROPERTY(config)
	bool bAllowGameplayControllerRumble;

	UPROPERTY(config)
	bool bInvertLook = false;

	UPROPERTY(config)
	float LookSensitivity = 1.f;
	
	UPROPERTY(config)
	bool AllowCameraFollowRollMovement = true;
	
	UPROPERTY(config)
	bool AllowToggleSprint = true;
	//******** Gameplay Collection Tab **********/////

	//******** Audio Collection Tab **********/////
	UPROPERTY(config)
	float OverallVolume;
	UPROPERTY(config)
	float MusicVolume;
	UPROPERTY(config)
	float SFXVolume;
	UPROPERTY(config)
	bool bAllowBackgroundAudio;
	UPROPERTY(config)
	bool bUseHDRAudioMode;
	//******** Audio Collection Tab **********/////
};
