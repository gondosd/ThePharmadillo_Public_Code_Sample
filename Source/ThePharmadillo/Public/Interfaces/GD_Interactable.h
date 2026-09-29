// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GD_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGD_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEPHARMADILLO_API IGD_Interactable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnInteracted();
};
