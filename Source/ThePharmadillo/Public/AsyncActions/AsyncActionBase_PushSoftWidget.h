// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncActionBase_PushSoftWidget.generated.h"

class UWidget_ActivatableWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidget_ActivatableWidgetBase*, PushedWidget);

UCLASS()
class THEPHARMADILLO_API UAsyncActionBase_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
	static UAsyncActionBase_PushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UWidget_ActivatableWidgetBase> InSoftWidgetClass,
		UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InWidgetStackTag,
		bool bFocusOnNewlyPushedWidget = true);

	//~ Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface


	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UWidget_ActivatableWidgetBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;

};
