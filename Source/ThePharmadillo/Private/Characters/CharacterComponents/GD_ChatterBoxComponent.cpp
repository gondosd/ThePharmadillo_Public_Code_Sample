// Gondos Daniel all rights reserved.


#include "Characters/CharacterComponents/GD_ChatterBoxComponent.h"
#include "DataAssets/GD_ChatterDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Widget_ChatterBox.h"


UGD_ChatterBoxComponent::UGD_ChatterBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGD_ChatterBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedChatterWidgetInstance = Cast<UWidget_ChatterBox>(GetUserWidgetObject());
	check(CachedChatterWidgetInstance);
}

void UGD_ChatterBoxComponent::PlayChatter(float PositivityValue)
{
	UGD_ChatterDataAsset* Chatter = GetChatter(GetPositivityLevel(PositivityValue));
	
	if (CachedChatterWidgetInstance && Chatter)
	{
		CachedChatterWidgetInstance->SetWidgetContent(Chatter);
		UGameplayStatics::SpawnSoundAttached(Chatter->ChatterSound.LoadSynchronous(), this);
	}
}

UGD_ChatterDataAsset* UGD_ChatterBoxComponent::GetChatter(EPositivityLevel PositivityLevel) const
{
	if (ChatterData.Num() == 0 || PositivityLevel == EPositivityLevel::Unknown) return nullptr;

	TArray<UGD_ChatterDataAsset*> FilteredChatterData = ChatterData.FilterByPredicate([PositivityLevel](const UGD_ChatterDataAsset* Data)
	{
		return Data->PositivityLevel == PositivityLevel;
	});

	if (FilteredChatterData.Num() == 0) return nullptr;

	//picks a random from chatter from the filtered list
	return FilteredChatterData[FMath::RandRange(0, FilteredChatterData.Num() - 1)];
}

EPositivityLevel UGD_ChatterBoxComponent::GetPositivityLevel(const float PositivityValue)
{
	const float ClampedValue = FMath::Clamp(PositivityValue, 0.0f, 1.0f);
	EPositivityLevel SelectedLevel = EPositivityLevel::Neutral;

	// how far we deviate from true neutral (0.5)
	// Deviation range: 0.0 at center -> 0.5 at extremes
	const float Deviation = FMath::Abs(ClampedValue - 0.5f);

	// Scaling factor: 0.0 (at 0.5) -> 1.0 (at 0.0 or 1.0)
	// At 0.25 and 0.75, nonNeutralChance becomes 0.5 (50% chance)
	if (FMath::FRand() < Deviation * 2.0f)
	{
		// Pick direction based on whether value is above or below 0.5
		SelectedLevel = (ClampedValue > 0.5f) ? EPositivityLevel::Happy : EPositivityLevel::Angry;
	}

	return SelectedLevel;
}
