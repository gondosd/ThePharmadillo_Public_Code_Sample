// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_HUD.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_HUD : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

	public:
	
	FORCEINLINE UWidget_ActivatableWidgetBase* GetInventoryWidgetReference(){return InventoryWidget;}
	
private:
#pragma region Bindigns

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidget_ActivatableWidgetBase> InventoryWidget;

#pragma endregion
};
