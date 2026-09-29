// Gondos Daniel all rights reserved.


#include "Actors/ActorComponents/GD_VolumeDebuggerDecal.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


void UGD_VolumeDebuggerDecal::OnRegister()
{
	Super::OnRegister();

#if UE_BUILD_SHIPPING
	// This debug component feature should be invisible in shipping builds
	SetVisibility(false);
#else
	SetVisibility(bShowDebugDecal);

	if (UBoxComponent* Box = GetOwner()->GetComponentByClass<UBoxComponent>())
	{
		SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

		FVector BoxExtent = Box->GetScaledBoxExtent();
		FVector ActorScale = GetOwner()->GetActorScale3D();
		//the rotated box is following actor scale too
		DecalSize = FVector(BoxExtent.Z/ ActorScale.X, BoxExtent.Y / ActorScale.Y, BoxExtent.X / ActorScale.Z); 

		SortOrder = MAX_int32 - static_cast<int32>(BoxExtent.X * BoxExtent.Y); //this shows the ingame volume priorities with decal render-order
	}
	
	SetDecalColor(VolumeDebuggerDecalColor.Get(FLinearColor::MakeRandomSeededColor(GetOwner()->GetActorLocation().Size2D())));

#endif
}
