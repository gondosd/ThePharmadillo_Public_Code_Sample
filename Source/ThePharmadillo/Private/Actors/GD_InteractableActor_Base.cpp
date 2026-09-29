// Gondos Daniel all rights reserved.


#include "Actors/GD_InteractableActor_Base.h"

#include "Actors/ActorComponents/GD_InteractionComponent.h"
#include "Components/WidgetComponent.h"


AGD_InteractableActor_Base::AGD_InteractableActor_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	InteractionComponent = CreateDefaultSubobject<UGD_InteractionComponent>("InteractionComponent");

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	InteractionWidget->SetVisibility(false);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetupAttachment(StaticMesh);
}
