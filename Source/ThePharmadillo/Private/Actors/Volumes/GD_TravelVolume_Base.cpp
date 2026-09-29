// Gondos Daniel all rights reserved.


#include "Actors/Volumes/GD_TravelVolume_Base.h"
#include "FrontendDebugHelper.h"
#include "Characters/GD_ThirdPersonCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

void AGD_TravelVolume_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (!LevelToOpen.IsNull())
	{
		FString LevelName;
		LevelToOpen.GetAssetName().Split(TEXT("Persistent_"), nullptr, &LevelName, ESearchCase::CaseSensitive, ESearchDir::FromStart);
		DescriptionText->SetText(FText::FromString(FString::Printf(TEXT("Travel To: %s \n Player Start: %s"), *LevelName, *PlayerStartTag)));
	}
#endif
}

void AGD_TravelVolume_Base::BeginPlay()
{
	Super::BeginPlay();
	if (LevelToOpen.IsNull())
	{
#if WITH_EDITOR
		Debug::Print(FString::Printf(TEXT("This volume has no Level to transition to: %s. Its being destroyed"), *this->GetActorLabel()));
#endif
		Destroy(); //if it has no LevelToOpen, makes no sense for it to exist
	}
}

void AGD_TravelVolume_Base::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor->IsA(AGD_ThirdPersonCharacter::StaticClass()))
	{
		FString Options = FString::Printf(TEXT("#%s"), *PlayerStartTag);;
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToOpen, true, Options);
	}
}
