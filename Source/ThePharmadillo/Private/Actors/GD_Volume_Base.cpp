// Gondos Daniel all rights reserved.


#include "Actors/GD_Volume_Base.h"

#include "Actors/ActorComponents/GD_VolumeDebuggerDecal.h"
#include "Characters/GD_ThirdPersonCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Subsystems/FrontendLoadingScreenSubsystem.h"


AGD_Volume_Base::AGD_Volume_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTriggerEndOverlap);
	RootComponent = TriggerVolume;

	VolumeDebuggerDecal = CreateDefaultSubobject<UGD_VolumeDebuggerDecal>(TEXT("VolumeDebuggerDecal"));
	VolumeDebuggerDecal->SetupAttachment(TriggerVolume);
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetRelativeScale3D(FVector(8.0f));
	Billboard->SetupAttachment(TriggerVolume);

	DescriptionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Descriptor"));
	DescriptionText->SetupAttachment(Billboard);
	
}

void AGD_Volume_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->SetGenerateOverlapEvents(true);
	TriggerVolume->SetLineThickness(10.f);
	
	VolumeDebuggerDecal->VolumeDebuggerDecalColor = VolumeDebuggerColor;

	Billboard->SetRelativeLocation(FVector(0.f, 0.f, TriggerVolume->GetScaledBoxExtent().Z / Transform.GetScale3D().Z)); //moves to the top of the volume
	
	DescriptionText->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	DescriptionText->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DescriptionText->SetRelativeScale3D(FVector((TriggerVolume->GetScaledBoxExtent()/ 1000.f).Size2D()));
	DescriptionText->SetHorizontalAlignment(EHTA_Center);
	DescriptionText->SetHiddenInGame(true);
	
	DescriptionText->SetTextRenderColor(VolumeDebuggerDecal->DecalColor.ToFColor(true));
	DescriptionText->SetText(FText::GetEmpty());
}

void AGD_Volume_Base::BeginPlay()
{
	Super::BeginPlay();

	UFrontendLoadingScreenSubsystem::Get(GetWorld())->OnLoadingScreenVisibilityUpdated.AddDynamic(this, &ThisClass::InitOverlapCheck);
}

void AGD_Volume_Base::InitOverlapCheck(bool bNewLoadingscreenVisible)
{
	if (!bNewLoadingscreenVisible)
	{
		TriggerVolume->GetOverlappingActors(OverlappingPlayerCharacters, AGD_ThirdPersonCharacter::StaticClass());
	}
}

void AGD_Volume_Base::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                            bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AGD_ThirdPersonCharacter::StaticClass()))
		OverlappingPlayerCharacters.AddUnique(OtherActor);
}

void AGD_Volume_Base::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGD_ThirdPersonCharacter::StaticClass()))
		OverlappingPlayerCharacters.RemoveSingle(OtherActor);
}
