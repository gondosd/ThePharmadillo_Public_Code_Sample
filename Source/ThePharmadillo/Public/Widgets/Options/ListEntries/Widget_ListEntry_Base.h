// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"


enum class ECommonInputType : uint8;
class UInputAction;
class UCommonTextBlock;
class UListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	virtual void NativeOnInitialized() override;
	
	
	void ResetOwningDataObject();
	
	//The Child widget blueprint should override this function for the gamepad interaction to function properly
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	
	//The Child widget blueprint should override this function to handle the highlight state when this entry widget is hovered or selected
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Toggle Entry Widget Highlight State"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;
	
	//The Child widget blueprint should override this function to handle the disabled state
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Toggle Entry Widget Disabled State"))
	void BP_OnToggleEntryWidgetInteractableState(bool bInIsInteractable) const;
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	//The child class should override this function to handle the initialization needed. super call is expected
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);

	//The child class should override this function to update the UI values after the data object has been modified. Super call is expected
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason);
	
	virtual void OnInputTypeChanged(ECommonInputType CommonInput);
	
	void OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData, EOptionsListDataModifiedReason ModifiedReason);
	
	//The child class should override this to change the editable state of the widget it owns. Super call is expected
	virtual void OnToggleEditableState(bool bIsEditable);
	
	void SelectThisEntryWidget();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsInteractable() const{return bIsInteractable;}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsInteractable(const bool bInIsInteractable)
	{
		BP_OnToggleEntryWidgetInteractableState(bInIsInteractable);
		bIsInteractable = bInIsInteractable;
	}
	
	UPROPERTY(Transient)
	TObjectPtr<UCommonInputSubsystem> CommonInputSubsystem;
		
private:
	void EvaluateResetBindingVisibility();
	
#pragma region BindWidgets
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	UCommonTextBlock* CommonText_SettingDisplayName;

#pragma endregion BindWidgets
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UInputAction* ResetAction;

	FUIActionBindingHandle ResetActionHandle;
	
	UPROPERTY(Transient)
	UListDataObject_Base* CachedOwningDataObject;
	
	bool bIsThisEntryWidgetSelected = false;
	
	bool bIsInteractable = true;
};
