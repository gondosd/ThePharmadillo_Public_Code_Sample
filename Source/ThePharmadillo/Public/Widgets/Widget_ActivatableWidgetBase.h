// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableWidgetBase.generated.h"

class UWidget_ActivatableWidgetBase;
class AGD_PlayerController;

//Similar to OnDeactivated, but this fires only when we step back from this widget. 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetClosed, UWidget_ActivatableWidgetBase*);

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_ActivatableWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnWidgetClosed OnWidgetClosed;
	
	//Fires the OnWidgedClosed delegate, so we can distinguish between deactivated and closed
	UFUNCTION(BlueprintCallable)
	void CloseWidget();

protected:
	virtual bool NativeOnHandleBackAction() override;

	UFUNCTION(BlueprintPure)
	AGD_PlayerController* GetOwningFrontendPlayerController();

private:
	TWeakObjectPtr<AGD_PlayerController> CachedOwningFrontendPC;
};
