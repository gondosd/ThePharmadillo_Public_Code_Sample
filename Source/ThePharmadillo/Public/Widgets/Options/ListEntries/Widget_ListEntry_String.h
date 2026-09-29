// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UFrontendCommonRotator;
class UFrontendCommonButtonBase;
class UListDataObject_String;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	//virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;

private:
	void OnNextOptionButtonClicked();
	void OnPreviousOptionButtonClicked();
	
	void OnRotatorValueChanged(int Value, bool bUserInitiated);

private:
#pragma region BindWigets
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UFrontendCommonButtonBase* CommonButton_NextOption;
#pragma endregion
	
	UPROPERTY(Transient)
	UListDataObject_String* CachedOwningStringDataObject;
};
