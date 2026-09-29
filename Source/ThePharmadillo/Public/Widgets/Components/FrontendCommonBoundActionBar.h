// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionBar.h"
#include "FrontendCommonBoundActionBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisualUpdated);

/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendCommonBoundActionBar : public UCommonBoundActionBar
{
	GENERATED_BODY()

protected:
	virtual void ActionBarUpdateEndImpl() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnVisualUpdated OnVisualUpdated;
	
};
