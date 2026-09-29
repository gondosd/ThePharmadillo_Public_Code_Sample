// Gondos Daniel all rights reserved.


#include "Characters/AI/Tasks/GD_BTT_PickRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UGD_BTT_PickRandomLocation::UGD_BTT_PickRandomLocation()
{
	NodeName = TEXT("Pick Random Location");
	bNotifyTick = false; // no tick
 
	// Vector-type Blackboard keys are selectable for this
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UGD_BTT_PickRandomLocation, TargetLocationKey));

}

FString UGD_BTT_PickRandomLocation::GetStaticDescription() const
{
	return FString::Printf(
	TEXT("Pick random point within %.0f units, write to %s"),
	SearchRadius,
	*TargetLocationKey.SelectedKeyName.ToString());
}

EBTNodeResult::Type UGD_BTT_PickRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
 
	if (!ControlledPawn || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
 
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ControlledPawn->GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}
 
	FNavLocation ResultLocation;
	const bool bFound = NavSys->GetRandomReachablePointInRadius(
		ControlledPawn->GetActorLocation(),
		SearchRadius,
		ResultLocation);
 
	if (!bFound)
	{
		return EBTNodeResult::Failed;
	}
 
	BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, ResultLocation.Location);
	return EBTNodeResult::Succeeded;

}
