// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"
#include "GD_PlayerController.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class THEPHARMADILLO_API AGD_PlayerController : public APlayerController , public IFrontendLoadingScreenInterface
{
	GENERATED_BODY()

protected:
	// ~ Begin APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	// ~ End APlayerController Interface

public:
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "IMC to register"))
	const UInputMappingContext* IMC;
	
private:
	static bool IsRunningOnSteamDeck();
};
