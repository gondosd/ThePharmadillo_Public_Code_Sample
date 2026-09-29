// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GD_GameplayEffect_Base.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class THEPHARMADILLO_API UGD_GameplayEffect_Base : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Activate(const UObject* WorldContextObject);
};
