// Gondos Daniel all rights reserved.


#include "Characters/AI/Tasks/GD_BTT_LookAtPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UGD_BTT_LookAtPlayer::UGD_BTT_LookAtPlayer()
{
	NodeName = TEXT("Look At Player");
	bNotifyTick = true;
	
}

FString UGD_BTT_LookAtPlayer::GetStaticDescription() const
{
	return FString::Printf(
	TEXT("Face player character (interp speed %.1f)"),
	RotationInterpSpeed);

}

EBTNodeResult::Type UGD_BTT_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
 
	if (!AIController || !AIController->GetPawn() || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;

}

void UGD_BTT_LookAtPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
 
	if (!ControlledPawn || !BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (!PlayerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	bool NPCRotationFinished = RotateCharacter(ControlledPawn, PlayerPawn, DeltaSeconds);
	bool PlayerRotationFinished = RotateCharacter(PlayerPawn, ControlledPawn, DeltaSeconds);
	
	if (NPCRotationFinished && PlayerRotationFinished)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}

bool UGD_BTT_LookAtPlayer::RotateCharacter(AActor* ActorToRotate, AActor* ActorToFaceTo, float DeltaTime)
{
	const FRotator CurrentRotation = ActorToRotate->GetActorRotation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(
		ActorToRotate->GetActorLocation(),
		ActorToFaceTo->GetActorLocation());
 
	if (bYawOnly)
	{
		DesiredRotation.Pitch = CurrentRotation.Pitch;
		DesiredRotation.Roll = CurrentRotation.Roll;
	}
 
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, RotationInterpSpeed);
	ActorToRotate->SetActorRotation(NewRotation);
	
	return CurrentRotation.Equals(DesiredRotation, 10.f);
}

