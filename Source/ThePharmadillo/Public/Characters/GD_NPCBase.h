// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GD_NPCBase.generated.h"

class UWidgetComponent;
class UGD_ChatterBoxComponent;
class UGD_InteractionComponent;

UCLASS()
class THEPHARMADILLO_API AGD_NPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGD_NPCBase();
	
	FORCEINLINE float GetPositivityValue() const {return PositivityFactor;}
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	float PositivityFactor = 0.f;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGD_InteractionComponent> InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> InteractionWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dialogue, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGD_ChatterBoxComponent> ChatterBoxWidget;
	
};
