// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "AsyncAction_Push_ConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);

UCLASS()
class THEPHARMADILLO_API UAsyncAction_Push_ConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,
		meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true",
			DisplayName = "Show Confirmation Screen"))
	static UAsyncAction_Push_ConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle,
		FText InScreenMessage,
		bool bInShouldPauseGame = true
	);

	virtual void Activate() override;
	
	
	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
	bool bShouldPauseGame;
};
