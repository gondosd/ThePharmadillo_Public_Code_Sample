// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GD_InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightChanged,bool,bHighlight);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPHARMADILLO_API UGD_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGD_InteractionComponent();
	
	virtual void InitializeComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnHighlightChanged OnChangeHighlightChanged;
	
	UFUNCTION()
	void Interact(APawn* InteractingActor) const;
	
	UFUNCTION(BlueprintCallable)
	void AddHighlight();
	UFUNCTION(BlueprintCallable)
	void RemoveHighlight();
	
	UFUNCTION()
	void SuppressHighlight(bool bIsGameFocused);

private:
	void CallInteractOnOwner() const;
	
	
	void EvaluateHighlight();
	void ChangeHighlightInternal(bool bHighlight) const;
	
	bool bIsHUDInitialized = false; //the first push on the HUD shouldnt affect the highlightcount
	bool bHasGameFocus = false; //checks real transitions
	int32 HighlightRefCount = 0;
};
