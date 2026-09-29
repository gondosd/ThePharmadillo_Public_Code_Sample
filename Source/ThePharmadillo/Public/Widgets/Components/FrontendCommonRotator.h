// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	void SetSelectedOptionByText(const FText& InTextOption);
};
