// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "GD_ChatterDataAsset.generated.h"


UCLASS(Blueprintable)
class THEPHARMADILLO_API UGD_ChatterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DialogueID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> DialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> ChatterSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPositivityLevel PositivityLevel = EPositivityLevel::Neutral;
};
