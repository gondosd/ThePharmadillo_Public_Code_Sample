// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GD_InteractableActor_Base.h"
#include "GameFramework/Actor.h"
#include "Subsystems/FrontendLoadingScreenSubsystem.h"
#include "GD_CollectableActor_Base.generated.h"

class UWidgetComponent;
class UGD_InteractionComponent;
class UGD_Item_DataAsset;

UCLASS(Abstract)
class THEPHARMADILLO_API AGD_CollectableActor_Base : public AGD_InteractableActor_Base
{
	GENERATED_BODY()

public:
	AGD_CollectableActor_Base();
	UFUNCTION(BlueprintCallable)
	void SetStaticMesh() const;
	
	virtual void OnInteracted_Implementation() override;
	
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UGD_Item_DataAsset* ItemType;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnGameLoaded(USaveSlot* Slot);	
	
};
