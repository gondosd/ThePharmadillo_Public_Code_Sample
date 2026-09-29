// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GD_PlayerStateBase.generated.h"

class UGD_InventoryComponent;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API AGD_PlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	AGD_PlayerStateBase();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UGD_InventoryComponent* GetInventoryComponent() const { return Inventory; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGD_InventoryComponent* Inventory;
};
