// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/GD_Volume_Base.h"
#include "GD_TravelVolume_Base.generated.h"

class UGD_VolumeDebuggerDecal;
class UBoxComponent;

UCLASS(Abstract, BlueprintType, Blueprintable)
class THEPHARMADILLO_API AGD_TravelVolume_Base : public AGD_Volume_Base
{
	GENERATED_BODY()
	
public:
	//Set the volume specific variables here
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading")
	TSoftObjectPtr<UWorld> LevelToOpen;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading")
	FString PlayerStartTag = FString("default");
	
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult) override;
	
};
