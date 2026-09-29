// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GD_BTS_TriggerChatter.generated.h"

/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UGD_BTS_TriggerChatter : public UBTService
{
	GENERATED_BODY()
	
public:
	UGD_BTS_TriggerChatter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
