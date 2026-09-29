// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GD_GameAreaSubsystem.generated.h"

class UGD_Area_DataAsset;
class AGD_MapAreaVolume_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAreaChanged, const UGD_Area_DataAsset*, AreaData);

UCLASS()
class THEPHARMADILLO_API UGD_GameAreaSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UGD_GameAreaSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAreaChanged OnAreaChangedEvent;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GameAreas)
	UGD_Area_DataAsset* GetCurrentAreaData();

	UFUNCTION(BlueprintCallable, Category = GameAreas)
	void AddArea(UGD_Area_DataAsset* NewArea);

	UFUNCTION(BlueprintCallable, Category = GameAreas)
	void RemoveArea(UGD_Area_DataAsset* Area);

private:
	void AreaChanged(const UGD_Area_DataAsset* NewArea);
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGD_Area_DataAsset>> CurrentAreas;
	
	UPROPERTY(Transient)
	TObjectPtr<UGD_Area_DataAsset> FallbackArea;
};
