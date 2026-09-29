// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"
#include "GD_HUD.generated.h"


class UWidget_ActivatableWidgetBase;
class UWidget_PrimaryLayout;

UCLASS(Abstract)
class THEPHARMADILLO_API AGD_HUD : public AHUD , public IFrontendLoadingScreenInterface
{
	GENERATED_BODY()

public:
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UWidget_PrimaryLayout> PrimaryLayoutWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSoftClassPtr<UWidget_ActivatableWidgetBase> HUDWidgetClass;
	
	UPROPERTY()
	UWidget_PrimaryLayout* InCreatedWidget;
};
