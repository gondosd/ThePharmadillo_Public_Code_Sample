// Gondos Daniel all rights reserved.


#include "Actors/ActorComponents/GD_InteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/GD_Interactable.h"
#include "Subsystems/FrontendUISubsystem.h"

UGD_InteractionComponent::UGD_InteractionComponent()
{
	bWantsInitializeComponent = true;
}

void UGD_InteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(this))
		UISubsystem->OnGameFocusChangedDelegate.AddDynamic(this, &ThisClass::SuppressHighlight);
}

void UGD_InteractionComponent::Interact(APawn* InteractingActor) const
{
	CallInteractOnOwner();
}

void UGD_InteractionComponent::CallInteractOnOwner() const
{
	const bool OwnerImplementsInterface = GetOwner()->GetClass()->ImplementsInterface(UGD_Interactable::StaticClass());
	checkf(OwnerImplementsInterface, TEXT("WARNING Owner doesnt implement the interactable interface"));
	IGD_Interactable::Execute_OnInteracted(GetOwner());
}

void UGD_InteractionComponent::EvaluateHighlight()
{
	const bool bIsHighlighted = HighlightRefCount > 0;

	ChangeHighlightInternal(bIsHighlighted);
	OnChangeHighlightChanged.Broadcast(bIsHighlighted);
}

void UGD_InteractionComponent::ChangeHighlightInternal(bool bHighlight) const
{
	for (auto Component : GetOwner()->GetComponents())
	{
		//Highlight meshes
		if (UMeshComponent* Mesh = Cast<UMeshComponent>(Component))
		{
			Mesh->SetRenderCustomDepth(bHighlight);
			Mesh->SetCustomDepthStencilValue(1);
		}

		//Interaction Widget handling
		if (UWidgetComponent* Widget = Cast<UWidgetComponent>(Component))
		{
			if (!Widget->ComponentHasTag(FName(TEXT("HighlightIndependent"))))
				Widget->SetVisibility(bHighlight);
		}
	}
}

void UGD_InteractionComponent::AddHighlight()
{
	HighlightRefCount++;
	EvaluateHighlight();
}

void UGD_InteractionComponent::RemoveHighlight()
{
	HighlightRefCount--;
	EvaluateHighlight();
}

void UGD_InteractionComponent::SuppressHighlight(bool bIsGameFocused)
{
	if (bIsGameFocused)
	{
		if (bIsHUDInitialized)
			if (!bHasGameFocus)
				AddHighlight();

		bIsHUDInitialized = true;
	}
	else
	{
		if (bHasGameFocus)
			RemoveHighlight();
	}

	bHasGameFocus = bIsGameFocused;
}
