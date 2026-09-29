// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GD_BTT_PickRandomLocation.generated.h"

UCLASS()
class THEPHARMADILLO_API UGD_BTT_PickRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGD_BTT_PickRandomLocation();
 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
 
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "PickRandomLocation", meta = (ClampMin = "0.0"))
	float SearchRadius = 1000.f;

};
