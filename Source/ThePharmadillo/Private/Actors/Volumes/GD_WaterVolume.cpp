// Gondos Daniel all rights reserved.


#include "Actors/Volumes/GD_WaterVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AGD_WaterVolume::AGD_WaterVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water Mesh"));
	WaterMesh->SetupAttachment(RootComponent);
}

void AGD_WaterVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	TriggerVolume->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
	WaterMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WaterMesh->SetRelativeLocation(FVector(0.f, 0.f, (TriggerVolume->GetScaledBoxExtent().Z / Transform.GetScale3D().Z)+ 300.f)); //moves to the top of the volume
	WaterMesh->SetRelativeScale3D(FVector((TriggerVolume->GetScaledBoxExtent()/ Transform.GetScale3D() / 50.f)));
}

void AGD_WaterVolume::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	UGameplayStatics::SpawnSoundAttached(SplashSound.LoadSynchronous(), OtherComp);
}


