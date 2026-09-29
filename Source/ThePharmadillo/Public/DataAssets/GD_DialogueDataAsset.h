// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GD_DialogueDataAsset.generated.h"
/**
 * 
 */
UCLASS(BlueprintType)
class THEPHARMADILLO_API UGD_DialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DialogueID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const TSoftObjectPtr<UTexture2D> SpeakerPicture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpeakerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName , FText> AnswerOptions;
};
