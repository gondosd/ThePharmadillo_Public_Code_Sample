// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GD_Volume_Base.generated.h"

class UTextRenderComponent;
class UGD_VolumeDebuggerDecal;
class UBoxComponent;
class UBillboardComponent;

UCLASS(Abstract, BlueprintType, Blueprintable)
class THEPHARMADILLO_API AGD_Volume_Base : public AActor
{
	GENERATED_BODY()

public:
	AGD_Volume_Base();
	
	//Set the volume specific variables here
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void InitOverlapCheck(bool bNewLoadingscreenVisible);
	
	UFUNCTION()
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                   bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = true))
	TOptional<FLinearColor> VolumeDebuggerColor;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Area")
	TObjectPtr<UBoxComponent> TriggerVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TObjectPtr<UBillboardComponent> Billboard;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TObjectPtr<UTextRenderComponent> DescriptionText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug", meta = (ShowOnlyInnerProperties))
	TObjectPtr<UGD_VolumeDebuggerDecal> VolumeDebuggerDecal;
	
	UPROPERTY(Transient)
	TArray<AActor*> OverlappingPlayerCharacters;
	
};
