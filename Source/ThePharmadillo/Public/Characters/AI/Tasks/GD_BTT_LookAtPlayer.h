// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GD_BTT_LookAtPlayer.generated.h"

UCLASS()
class THEPHARMADILLO_API UGD_BTT_LookAtPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGD_BTT_LookAtPlayer();
 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
 
protected:
	
	bool RotateCharacter(AActor* ActorToRotate, AActor* ActorToFaceTo, float DeltaTime);
 
	/** Interpolation speed passed to RInterpTo — higher snaps faster, lower is smoother/slower. */
	UPROPERTY(EditAnywhere, Category = "LookAtPlayer", meta = (ClampMin = "0.0"))
	float RotationInterpSpeed = 8.f;
 
	/** If true, only yaw is adjusted so the pawn doesn't pitch up/down to face the target. */
	UPROPERTY(EditAnywhere, Category = "LookAtPlayer")
	bool bYawOnly = true;

	
	
};
