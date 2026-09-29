// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "GD_VolumeDebuggerDecal.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPHARMADILLO_API UGD_VolumeDebuggerDecal : public UDecalComponent
{
public:
	 
	virtual void OnRegister() override;
	
	//This component is Editor only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bShowDebugDecal = false;
	
	UPROPERTY()
	TOptional<FLinearColor> VolumeDebuggerDecalColor;
	
private:
	GENERATED_BODY()
	
	
	
};
