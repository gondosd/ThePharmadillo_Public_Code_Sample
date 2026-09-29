// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GD_InteractionHandlerComponent.generated.h"


class AGD_ThirdPersonCharacter;
class UGD_InteractionComponent;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPHARMADILLO_API UGD_InteractionHandlerComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<APawn> PawnOwner = nullptr;
	
	UPROPERTY()
	TObjectPtr<UGD_InteractionComponent> MostFittingActorComponent = nullptr;

private:

	UPROPERTY()
	TMap<UGD_InteractionComponent*, float> InteractableActorComponents;
	
public:
	UGD_InteractionHandlerComponent();

	UFUNCTION(BlueprintCallable)
	void TryInteract(APawn* InteractingActor);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void NewActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);
	UFUNCTION()
	void ActorLeftOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

private:
	void InitOverlappingActorCheck();
	void SelectMostFittingOverlappingActor();
	
};
