// Gondos Daniel all rights reserved.


#include "Characters/AI/Services/GD_BTS_TriggerChatter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CharacterComponents/GD_ChatterBoxComponent.h"

UGD_BTS_TriggerChatter::UGD_BTS_TriggerChatter()
{
	NodeName = TEXT("Trigger Chatter");
	Interval = 20.f;
	RandomDeviation = 5.f;
	bCallTickOnSearchStart = false;
}

void UGD_BTS_TriggerChatter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Shouldn't bark during a conversation
	if (const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent(); BB && BB->GetValueAsBool(TEXT("bConversation"))) return; 

	if (UGD_ChatterBoxComponent* ChatterBoxComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UGD_ChatterBoxComponent>())
		ChatterBoxComponent->PlayChatter(.25f ); //TODO: get this data from the NPC character
}
