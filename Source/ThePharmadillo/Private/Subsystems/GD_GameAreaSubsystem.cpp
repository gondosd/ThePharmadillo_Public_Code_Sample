// Gondos Daniel all rights reserved.


#include "Subsystems/GD_GameAreaSubsystem.h"

#include "PharmadilloFunctionLibrary.h"
#include "SaveManager.h"
#include "DataAssets/GD_Area_DataAsset.h"
#include "Subsystems/FrontendAudioSubsystem.h"

UGD_GameAreaSubsystem* UGD_GameAreaSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return World->GetWorld()->GetSubsystem<UGD_GameAreaSubsystem>();
	}

	return nullptr;
}

bool UGD_GameAreaSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void UGD_GameAreaSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	GetWorld()->GetGameInstance()->GetSubsystem<USaveManager>()->OnRequestAreaName.BindLambda([this]()
	{
		return GetCurrentAreaData()->AreaName;
	});
}

UGD_Area_DataAsset* UGD_GameAreaSubsystem::GetCurrentAreaData()
{
	if (CurrentAreas.IsEmpty())
	{
		FallbackArea = NewObject<UGD_Area_DataAsset>();
		FallbackArea->AreaName = FText::FromString(UPharmadilloFunctionLibrary::GetCurrentLevelNameWithoutPersistentPrefix(GetWorld()));
		return FallbackArea;
	}

	return CurrentAreas.Last();
}

void UGD_GameAreaSubsystem::AddArea(UGD_Area_DataAsset* NewArea)
{
	if (!NewArea) return;
	const UGD_Area_DataAsset* PreviousTopArea = GetCurrentAreaData();
	CurrentAreas.Add(NewArea);
	CurrentAreas.Sort([](const UGD_Area_DataAsset& LHS, const UGD_Area_DataAsset& RHS) { return LHS > RHS; });

	const UGD_Area_DataAsset* NewTopArea = GetCurrentAreaData();
	if (NewTopArea != PreviousTopArea)
		AreaChanged(NewTopArea);
}


void UGD_GameAreaSubsystem::RemoveArea(UGD_Area_DataAsset* Area)
{
	if (!Area) return;
	const UGD_Area_DataAsset* PreviousTopArea = GetCurrentAreaData();
	CurrentAreas.RemoveSingle(Area);

	const UGD_Area_DataAsset* NewTopArea = GetCurrentAreaData();
	if (NewTopArea != PreviousTopArea)
		AreaChanged(NewTopArea);
}

void UGD_GameAreaSubsystem::AreaChanged(const UGD_Area_DataAsset* NewArea)
{
	OnAreaChangedEvent.Broadcast(NewArea);
	UFrontendAudioSubsystem::Get(this)->LazyPlayMusic(NewArea->AreaMusic, false);
}
