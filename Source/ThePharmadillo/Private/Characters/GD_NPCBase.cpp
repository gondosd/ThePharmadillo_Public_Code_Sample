// Gondos Daniel all rights reserved.


#include "Characters/GD_NPCBase.h"

#include "Actors/ActorComponents/GD_InteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/CharacterComponents/GD_ChatterBoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AGD_NPCBase::AGD_NPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Create interaction component
	InteractionComponent = CreateDefaultSubobject<UGD_InteractionComponent>(TEXT("InteractionComponent"));
	
	// Create the widget component that shows the input bindings
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetVisibility(false);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetupAttachment(GetRootComponent());
	
	// Create the widget component that shows the chatter dialogues
	ChatterBoxWidget = CreateDefaultSubobject<UGD_ChatterBoxComponent>(TEXT("ChatterBoxWidget"));
	ChatterBoxWidget->ComponentTags.Add("HighlightIndependent");
	ChatterBoxWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ChatterBoxWidget->SetDrawAtDesiredSize(true);
	ChatterBoxWidget->SetupAttachment(GetRootComponent());
	
}


