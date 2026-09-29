// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GD_Interactable.h"
#include "GD_InteractableActor_Base.generated.h"

class UWidgetComponent;
class UGD_InteractionComponent;
class UGD_Item_DataAsset;

UCLASS(Abstract)
class THEPHARMADILLO_API AGD_InteractableActor_Base : public AActor, public IGD_Interactable
{
	GENERATED_BODY()

public:
	AGD_InteractableActor_Base();

protected:	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGD_InteractionComponent* InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* InteractionWidget;
	
};
