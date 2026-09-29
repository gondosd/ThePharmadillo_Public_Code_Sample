// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/GD_DialogueDataAsset.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GD_AsyncAction_Push_DialogueScreen.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueScreenButtonClickedDelegate, FName, AnswerID);

UCLASS()
class THEPHARMADILLO_API UGD_AsyncAction_Push_DialogueScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,
		meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", DefaultToSelf = "Instigator", BlueprintInternalUseOnly = "true",
			DisplayName = "Show Dialogue Screen"))
	static UGD_AsyncAction_Push_DialogueScreen* PushDialogueScreen(
		const UObject* WorldContextObject,
		const UGD_DialogueDataAsset* DialogueDataAsset,
		const AActor* Instigator
	);

	virtual void Activate() override;
	
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueScreenButtonClickedDelegate OnButtonClicked;

private:
	void SetConversationFlag(bool bValue) const;
	
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	
	UPROPERTY()
	const UGD_DialogueDataAsset* CachedDialogueDataAsset;
	
	UPROPERTY()
	const AActor* Instigator;
};
