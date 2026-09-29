// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GD_PawnBase.generated.h"

class UInputMappingContext;

UCLASS()
class THEPHARMADILLO_API AGD_PawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGD_PawnBase();
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;
};
