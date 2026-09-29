// Gondos Daniel all rights reserved.


#include "Subsystems/FrontendLoadingScreenSubsystem.h"

#include "PreLoadScreenManager.h"
#include "SaveManager.h"
#include "Blueprint/UserWidget.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"
#include "Subsystems/GD_LevelTransitionSubsystem.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UFrontendLoadingScreenSubsystem* UFrontendLoadingScreenSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendLoadingScreenSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never; //if there are children subsystems from this, this shouldnt tick
	}
	return ETickableTickType::Conditional;
}

bool UFrontendLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	UGD_LevelTransitionSubsystem::Get(GetWorld())->CacheClassesForTravel(); // caches player controller, player state, player character for level transition

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UFrontendLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		UGD_LevelTransitionSubsystem::Get(GetWorld())->LoadCachedDataAfterTravel(); //restores cached classes after level transition
		bIsCurrentlyLoadingMap = false;
	}
}

void UFrontendLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
#if WITH_EDITOR
	if (GEditor && GEditor->bIsSimulatingInEditor) return; //Simulate shouldnt display loading screen
#endif
	
	//check if there's any start up loading screen that's currently active
	if (IsPreLoadScreenActive())
	{
		return;
	}

	//check if we should show the loading screen
	if (ShouldShowLoadingScreen())
	{
		// try display the loading screen here
		TryDisplayLoadingScreenIfNone();

		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason); //TODO this shouldn't be broadcasted in tick, if its not changing, it shouldn't be broadcasted
	}
	else
	{
		//try remove the current active loading screen
		TryRemoveLoadingScreen();
		HoldLoadingScreenStartUpTime = -1.f;

		//Notify the loading is complete
		NotifyLoadingScreenVisibilityChanged(false);

		//disable the ticking
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive()
{
	if (const FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}
	return false;
}

bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();

	//check if the objects in the world need a loading screen
	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for texture streaming");

	//there is no need to show the loading screen. allow the world to be rendered to our viewport here
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	float HoldLoadingScreenExtraSeconds = (GIsEditor && !LoadingScreenSettings->bShowElongatedLoadingScreenInEditor)
		                                      ? SMALL_NUMBER
		                                      : LoadingScreenSettings->HoldLoadingScreenExtraSeconds;

	if (ElapsedTime < HoldLoadingScreenExtraSeconds)
	{
		return true;
	}

	return false;
}

bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	//we can check here if the components are ready
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	const UWorld* OwningWorld = GetGameInstance()->GetWorld();
	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasnt begun play yet");
		return true;
	}

	if (!OwningWorld->GetFirstLocalPlayerFromController())
	{
		CurrentLoadingReason = TEXT("Player controller is under initialization");
		return true;
	}

	//TODO: check if game states, player states, or player character, actor components are ready

	if (!OwningWorld->GetGameState())
	{
		CurrentLoadingReason = TEXT("Game State is under construction");
		return true;
	}

	if (OwningWorld->GetGameState()->PlayerArray.Num() == 0 || !OwningWorld->GetGameState()->PlayerArray[0])
	{
		CurrentLoadingReason = TEXT("Player State is under construction");
		return true;
	}

	if (!OwningWorld->GetFirstPlayerController()->GetPawn())
	{
		CurrentLoadingReason = TEXT("Waiting for Character...");
		return true;
	}

	return false;
}

void UFrontendLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	//if there is already active loading screen, return early
	if (CachedCreatedLoadingScreenWidget)
		return;

	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, FName("LoadingScreen"));
	check(CreatedWidget);

	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef(), 1000);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UFrontendLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
		return;

	if (CachedSaveFile != NAME_None) //this is a hack so there wont be any crashes on loading
	{
		USaveManager::Get(this)->LoadSlot(
			CachedSaveFile,
			FOnGameLoaded::CreateLambda([this](USaveSlot* Slot)
				{
					if (Slot)
					{
						GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());
						CachedCreatedLoadingScreenWidget.Reset();
					}
				}
			)
		);

		CachedSaveFile = NAME_None;
	}
	else
	{
		GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());
		CachedCreatedLoadingScreenWidget.Reset();
	}
}

void UFrontendLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	OnLoadingScreenVisibilityUpdated.Broadcast(bIsVisible);
	
	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		if (GM->Implements<UFrontendLoadingScreenInterface>())
		{
			if (bIsVisible)
			{
				IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(GM);
			}
			else
			{
				IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(GM);
			}
		}
	}
	
	// For all existing player relevant objects
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
			continue;


		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			//Query if the player controller implements the interfacee. Call the function through interface to notify the loading status if yes
			if (PC->Implements<UFrontendLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UFrontendLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}

			if (AHUD* HUD = PC->GetHUD())
			{
				if (HUD->Implements<UFrontendLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(HUD);
					}
					else
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(HUD);
					}
				}
			}

			
			
			//The code for notifying other objects in the world goes here
		}
	}
}
