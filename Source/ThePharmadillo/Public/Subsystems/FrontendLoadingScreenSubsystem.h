// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingRason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityUpdatedDelegate, bool , bNewVisibility);


class USaveSlot;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static UFrontendLoadingScreenSubsystem* Get(const UObject* WorldContextObject);
	
	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnLoadingScreenVisibilityUpdatedDelegate OnLoadingScreenVisibilityUpdated;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	//SaveSystem hacky workaround because plugin is not working right
	FORCEINLINE void CacheLoadingFile(FName SaveSlotName) {CachedSaveFile = SaveSlotName;}

	//tickable section:
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnMapPostLoaded(UWorld* LoadedWorld);

	void TryUpdateLoadingScreen();

	static bool IsPreLoadScreenActive();
	
	bool ShouldShowLoadingScreen();
	
	bool CheckTheNeedToShowLoadingScreen();
	
	void TryDisplayLoadingScreenIfNone();
	
	void TryRemoveLoadingScreen();
	
	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);
	
	UPROPERTY()
	FName CachedSaveFile;

	bool bIsCurrentlyLoadingMap = false;
	
	float HoldLoadingScreenStartUpTime = -1.f;
	
	FString CurrentLoadingReason;
	
	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
};
