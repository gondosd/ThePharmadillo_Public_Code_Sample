// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendUISubsystem.generated.h"

class UGD_DialogueDataAsset;
class UWidget_PrimaryLayout;
class UWidget_ActivatableWidgetBase;
struct FGameplayTag;
class UFrontendCommonButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UFrontendCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFocusChangedDelegate, bool, bIsGameFocused);


UCLASS()
class THEPHARMADILLO_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableWidgetBase> InSoftWidgetClass,
	                                TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableWidgetBase*)> AsyncPushStateCallback);
	void PushConfirmScreenToModalStackAsynch(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg,
	                                         TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback, bool bShouldPauseGame = true);

	void PushDialogueScreenToModalStackAsync(const UGD_DialogueDataAsset& InDialogueDataAsset,
	                                         TFunction<void(FName)> ButtonClickedCallback);

	UFUNCTION(BlueprintCallable)
	void ClearEntireWidgetStackByTag(UPARAM(meta = (Categories = "Frontend.WidgetStack")) const FGameplayTag InTag) const;

	UFUNCTION(BlueprintPure)
	bool CheckWidgetStackIfEmptyByTag(UPARAM(meta = (Categories = "Frontend.WidgetStack")) const FGameplayTag InTag) const;

protected:
	void HandleActiveInputModeChanged(ECommonInputMode CommonInputMode);

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameFocusChangedDelegate OnGameFocusChangedDelegate;

private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
