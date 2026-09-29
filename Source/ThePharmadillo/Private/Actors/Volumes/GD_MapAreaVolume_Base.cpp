// Gondos Daniel all rights reserved.


#include "Actors/Volumes/GD_MapAreaVolume_Base.h"

#include "FrontendDebugHelper.h"
#include "Characters/GD_ThirdPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "DataAssets/GD_Area_DataAsset.h"
#include "Subsystems/GD_GameAreaSubsystem.h"


void AGD_MapAreaVolume_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
#if WITH_EDITOR
	if (AreaData)
		DescriptionText->SetText(AreaData->AreaName);
#endif
}

void AGD_MapAreaVolume_Base::BeginPlay()
{
	Super::BeginPlay();

	if (AreaData)
	{
		AreaData->AreaSize = TriggerVolume->GetScaledBoxExtent().X * TriggerVolume->GetScaledBoxExtent().Y; //Necessary for priorization on overlapping triggers
		AreaData->VolumeLocation = GetActorLocation(); //Necessary so overlapping same name volumes can be handled as one area
	}
	else
	{
#if WITH_EDITOR
		Debug::Print(FString::Printf(TEXT("This volume has no AreaData: %s. Its being destroyed"), *this->GetActorLabel()));
#endif
		Destroy(); //if it has no AreaData, makes no sense for it to exist
	}
}

void AGD_MapAreaVolume_Base::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//for streaming out sublevels
	if (EndPlayReason == EEndPlayReason::LevelTransition || EndPlayReason == EEndPlayReason::RemovedFromWorld)
		UGD_GameAreaSubsystem::Get(GetWorld())->RemoveArea(AreaData);

	Super::EndPlay(EndPlayReason);
}

void AGD_MapAreaVolume_Base::InitOverlapCheck(bool bNewLoadingscreenVisible)
{
	Super::InitOverlapCheck(bNewLoadingscreenVisible);
	
	if (!bNewLoadingscreenVisible)
	{
		if (!OverlappingPlayerCharacters.IsEmpty())
			UGD_GameAreaSubsystem::Get(GetWorld())->AddArea(AreaData);
	}

}

void AGD_MapAreaVolume_Base::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor->IsA(AGD_ThirdPersonCharacter::StaticClass()))
	{
		UGD_GameAreaSubsystem::Get(GetWorld())->AddArea(AreaData);
	}
}

void AGD_MapAreaVolume_Base::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                 int32 OtherBodyIndex)
{
	Super::OnTriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if (OtherActor->IsA(AGD_ThirdPersonCharacter::StaticClass()))
	{
		UGD_GameAreaSubsystem::Get(GetWorld())->RemoveArea(AreaData);
	}
}
