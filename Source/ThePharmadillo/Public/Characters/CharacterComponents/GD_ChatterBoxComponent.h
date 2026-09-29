// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "GD_ChatterBoxComponent.generated.h"

class UWidget_ChatterBox;
class UGD_ChatterDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPHARMADILLO_API UGD_ChatterBoxComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGD_ChatterBoxComponent();

	virtual void BeginPlay() override;

	void PlayChatter(float PositivityValue = 0.f);

private:

	UGD_ChatterDataAsset* GetChatter(EPositivityLevel PositivityLevel = EPositivityLevel::Neutral) const;

	static EPositivityLevel GetPositivityLevel(const float PositivityValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UGD_ChatterDataAsset*> ChatterData;
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_ChatterBox> CachedChatterWidgetInstance;
};
