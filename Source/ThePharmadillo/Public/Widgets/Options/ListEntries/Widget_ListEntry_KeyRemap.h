// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnalogSlider.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UListDataObject_KeyRemap;
class UFrontendCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason) override;

	virtual void OnInputTypeChanged(ECommonInputType CommonInput) override;
	
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	virtual void OnToggleEditableState(bool bIsEditable) override;

private:
	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
	
	
#pragma region BindWidgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget ,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_RemapKey;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget ,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_ResetKeyBinding;
#pragma endregion
	
	UPROPERTY(Transient)
	TObjectPtr<UListDataObject_KeyRemap> CachedOwningKeyRemapDataObject;
	
	
	
};
