// Gondos Daniel all rights reserved.


#include "Characters/CharacterComponents/GD_InteractionHandlerComponent.h"
#include "Actors/ActorComponents/GD_InteractionComponent.h"


UGD_InteractionHandlerComponent::UGD_InteractionHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGD_InteractionHandlerComponent::TryInteract(APawn* InteractingActor)
{
	if (MostFittingActorComponent)
		MostFittingActorComponent->Interact(InteractingActor);
}

void UGD_InteractionHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
		PawnOwner = Cast<APawn>(GetOwner());

	OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::NewActorOverlapped);
	OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::ActorLeftOverlap);
	
	InitOverlappingActorCheck();
}

void UGD_InteractionHandlerComponent::InitOverlappingActorCheck()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if (UGD_InteractionComponent* InteractComp = Cast<UGD_InteractionComponent>(OverlappingActor->GetComponentByClass(UGD_InteractionComponent::StaticClass())))
		{
			InteractableActorComponents.Add(InteractComp, 0.f);
		}
	}
}

void UGD_InteractionHandlerComponent::NewActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                         int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGD_InteractionComponent* InteractComp = Cast<UGD_InteractionComponent>(OtherActor->GetComponentByClass(UGD_InteractionComponent::StaticClass())))
	{
		InteractableActorComponents.Add(InteractComp, 0.f);
	}
}

void UGD_InteractionHandlerComponent::ActorLeftOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                       int OtherBodyIndex)
{
	if (UGD_InteractionComponent* InteractComp = Cast<UGD_InteractionComponent>(OtherActor->GetComponentByClass(UGD_InteractionComponent::StaticClass())))
	{
		if (InteractableActorComponents.Contains(InteractComp))
		{
			InteractableActorComponents.Remove(InteractComp);
		}
	}
}

void UGD_InteractionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SelectMostFittingOverlappingActor();
}

void UGD_InteractionHandlerComponent::SelectMostFittingOverlappingActor()
{
	if (InteractableActorComponents.Num() <= 0)
	{
		if (MostFittingActorComponent)
			MostFittingActorComponent->RemoveHighlight();
		MostFittingActorComponent = nullptr;
		return;
	}
	checkf(PawnOwner, TEXT("This Component is for pawns only because of look rotation check."))


	for (TPair<UGD_InteractionComponent*, float>& InteractComp : InteractableActorComponents)
	{
		const float DistanceSquared = FVector::DistSquared2D(PawnOwner->GetActorLocation(), InteractComp.Key->GetOwner()->GetActorLocation());
		const float NormalizedDistance = FMath::Clamp((1.0f - (DistanceSquared / FMath::Square(SphereRadius))), 0.0f, 1.0f);

		const FVector DirectionToActor = (InteractComp.Key->GetOwner()->GetActorLocation() - PawnOwner->GetActorLocation()).GetSafeNormal2D();
		const FVector LookDirection = PawnOwner->GetControlRotation().Vector().GetSafeNormal2D(); // which direction are you looking, not facing

		const float CurrentFitnessScore = NormalizedDistance + (LookDirection.Dot(DirectionToActor));

		InteractComp.Value = CurrentFitnessScore;


		const float BestScoreSoFar = InteractableActorComponents.FindRef(MostFittingActorComponent);

		if (BestScoreSoFar <= 1.f) //if we are facing away it should turn the highlight off
		{
			if (MostFittingActorComponent)
				MostFittingActorComponent->RemoveHighlight();
			MostFittingActorComponent = nullptr;
		}

		if ((CurrentFitnessScore > 1.f) && (CurrentFitnessScore > BestScoreSoFar)) //if we are facing away it shouldn't nominate for MostFitting
		{
			if (MostFittingActorComponent)
				MostFittingActorComponent->RemoveHighlight();
			MostFittingActorComponent = InteractComp.Key;
			MostFittingActorComponent->AddHighlight();
		}
	}
}
