// Gondos Daniel all rights reserved.


#include "FrameworkClasses/GD_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendAudioSubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void AGD_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();

	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		if (IsRunningOnSteamDeck()) //if we use a Steamdeck we force some settings
		{
			GameUserSettings->SetOverallScalabilityLevel(1); // e.g. Medium
			GameUserSettings->ApplyNonResolutionSettings();
			GameUserSettings->SetScreenResolution(FIntPoint(1280, 800));
			GameUserSettings->ApplySettings(false);
		}
		else
		{
			GameUserSettings->RunHardwareBenchmark();
			GameUserSettings->ApplyHardwareBenchmarkResults();
		}
	}
}

void AGD_PlayerController::OnLoadingScreenDeactivated_Implementation()
{
	IFrontendLoadingScreenInterface::OnLoadingScreenDeactivated_Implementation();

	UFrontendAudioSubsystem::InitializeAudioSettings();

	if (const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->GetUserSettings()->RegisterInputMappingContext(IMC);
	}
}

bool AGD_PlayerController::IsRunningOnSteamDeck()
{
	static const bool bIsSteamDeck = []() -> bool
	{
		const FString EnvValue = FPlatformMisc::GetEnvironmentVariable(TEXT("SteamDeck"));
		return EnvValue == TEXT("1");
	}();

	return bIsSteamDeck;
}
