// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/GD_Volume_Base.h"
#include "GD_ResetterVolume_Base.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class THEPHARMADILLO_API AGD_ResetterVolume_Base : public AGD_Volume_Base
{
	GENERATED_BODY()

protected:
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult) override;

};
