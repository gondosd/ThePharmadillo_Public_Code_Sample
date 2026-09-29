// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnalogSlider.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_OptionsScreen.generated.h"


class UInputAction;
class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
class UFrontendCommonListView;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_OptionsScreen : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();


	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;
	
	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifiedReason ModifiedReason);
	void EvaluateResetAllState(ECommonInputType CommonInput);

#pragma region BoundWidgets

	UPROPERTY(meta = (BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;

	UPROPERTY(meta = (BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;

	UPROPERTY(meta = (BindWidget))
	UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;

#pragma endregion BoundWidgets

	UPROPERTY(Transient)
	TObjectPtr<UCommonInputSubsystem> CommonInputSubsystem;
	
	//Handle the creation of data in the option screen. Direct access to this variable is forbidden
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> CachedFoundListSourceItems;
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UInputAction* ResetAction;

	FUIActionBindingHandle ResetActionHandle;
	
	UPROPERTY(transient)
	TArray<UListDataObject_Base*> ResettableDataArray;
	
	bool bIsResettingData = false;
};
