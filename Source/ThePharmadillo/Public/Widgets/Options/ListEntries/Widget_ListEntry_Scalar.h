// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UListDataObject_Scalar;
class UAnalogSlider;
class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifiedReason ModifiedReason) override;

	virtual void OnToggleEditableState(bool bIsEditable) override;
private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	
	UFUNCTION()
	void OnSliderMouseCaptureBegin();

#pragma region BindWigets
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UAnalogSlider* AnalogSlider_SettingSlider;

#pragma endregion
	
	UPROPERTY(Transient)
	UListDataObject_Scalar* CachedOwningScalarDataObject;
};
