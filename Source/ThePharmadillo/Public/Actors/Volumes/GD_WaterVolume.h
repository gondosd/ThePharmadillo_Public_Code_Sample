// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GD_ResetterVolume_Base.h"
#include "GD_WaterVolume.generated.h"

class UStaticMeshComponent;
class UParticleSystem;

UCLASS(Abstract, BlueprintType, Blueprintable)
class THEPHARMADILLO_API AGD_WaterVolume : public AGD_ResetterVolume_Base
{
	GENERATED_BODY()

public:
	AGD_WaterVolume();

	//Set the volume specific variables here
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> SplashSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WaterMesh;

};
