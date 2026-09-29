// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/GD_Volume_Base.h"
#include "GD_MapAreaVolume_Base.generated.h"

class UGD_VolumeDebuggerDecal;
class UGD_Area_DataAsset;
class UBoxComponent;

UCLASS(Abstract, BlueprintType, Blueprintable)
class THEPHARMADILLO_API AGD_MapAreaVolume_Base : public AGD_Volume_Base
{
	GENERATED_BODY()

public:
	//Set the volume specific variables here
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void InitOverlapCheck(bool bNewLoadingscreenVisible) override;
	
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGD_Area_DataAsset>  AreaData;
};
