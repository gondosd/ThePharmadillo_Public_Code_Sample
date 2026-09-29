// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GD_PlayerController.h"
#include "GD_GameplayPlayerController.generated.h"

class UWidget_ActivatableWidgetBase;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS(Abstract)
class THEPHARMADILLO_API AGD_GameplayPlayerController : public AGD_PlayerController
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetInventoryWidgetReference(UWidget_ActivatableWidgetBase* InInventoryWidgetReference) { InventoryWidget = InInventoryWidgetReference; }

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable)
	void OpenInventory();

private:
	void PausePanelClosed(UWidget_ActivatableWidgetBase* Widget_ActivatableWidgetBase);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidget_ActivatableWidgetBase> InventoryWidget;
};
