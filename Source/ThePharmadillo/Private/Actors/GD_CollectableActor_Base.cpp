// Gondos Daniel all rights reserved.


#include "Actors/GD_CollectableActor_Base.h"

#include "PharmadilloFunctionLibrary.h"
#include "SaveManager.h"
#include "Actors/ActorComponents/GD_InteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/GD_Item_DataAsset.h"


AGD_CollectableActor_Base::AGD_CollectableActor_Base()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AGD_CollectableActor_Base::BeginPlay()
{
	Super::BeginPlay();
	USaveManager::Get(this)->OnGameLoaded.AddUniqueDynamic(this, &AGD_CollectableActor_Base::OnGameLoaded);
	
	SetLifeSpan(FMath::RandRange(20.f, 22.f));
}

void AGD_CollectableActor_Base::OnGameLoaded(USaveSlot* Slot)
{
	SetStaticMesh();
}

void AGD_CollectableActor_Base::SetStaticMesh() const
{
	if (ItemType)
	{
		if (ItemType->HarvestedMeshes.IsEmpty())
			return;
		
		const auto Mesh = ItemType->HarvestedMeshes[FMath::RandRange(0, ItemType->HarvestedMeshes.Num() - 1)];
		StaticMesh->SetStaticMesh(Mesh.LoadSynchronous());
		StaticMesh->SetSimulatePhysics(true);
	}
}

void AGD_CollectableActor_Base::OnInteracted_Implementation()
{
	IGD_Interactable::OnInteracted_Implementation();
	
	UPharmadilloFunctionLibrary::AddItemToPlayer(this, ItemType);
	
	Destroy();
}
