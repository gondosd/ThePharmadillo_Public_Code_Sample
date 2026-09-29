// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GD_LevelTransitionSubsystem.generated.h"


USTRUCT()
struct FGD_CachedProperty
{
	GENERATED_BODY()
 
	UPROPERTY()
	FName PropertyName;
 
	TArray<uint8> RawBytes;
};


USTRUCT()
struct FGD_CachedActorData
{
	GENERATED_BODY()
 
	UPROPERTY()
	TObjectPtr<UClass> ActorClass = nullptr;
 
	TArray<FGD_CachedProperty> Properties;
	
	UPROPERTY()
	TArray<TObjectPtr<UObject>> ReferencedObjects;
};


/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UGD_LevelTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	static UGD_LevelTransitionSubsystem* Get(const UObject* WorldContextObject);
	
	void CacheClassesForTravel();
	
	void LoadCachedDataAfterTravel(bool bClearAfterLoad = true);
	

private:
	UPROPERTY()
	bool bHasCachedData = false;
	
	UPROPERTY()
	FGD_CachedActorData CapturedPCData;
	UPROPERTY()
	FGD_CachedActorData CapturedInventoryData;
	UPROPERTY()
	FGD_CachedActorData CapturedPlacerCharacterData;
	
	static void ForEachSaveGameProperty(const UClass* Class, TFunctionRef<void(FProperty*)> Func);
	
	static FGD_CachedActorData CaptureActor(UObject* Object);
	
	static int32 RestoreSingleActor(UObject* Object, const FGD_CachedActorData& CachedData);

	static void CollectObjectRefs(FProperty* Property, const void* ValuePtr, TArray<TObjectPtr<UObject>>& OutRefs);
};
