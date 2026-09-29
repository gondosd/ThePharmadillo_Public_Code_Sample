// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightButtonClicked);

class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);
	
	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText()const;
	
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);
	
	UFUNCTION(BlueprintCallable)
	void SetButtonDescriptionText(const FText InText);
	
	const FSlateBrush& GetButtonDisplayImage() const;

	
	DECLARE_EVENT(UCommonButtonBase, FCommonButtonEvent);
	FCommonButtonEvent& OnRightClicked() const { return OnRightClickedEvent; }
	
protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnCurrentTextStyleChanged() override;

	virtual void NativeOnHovered() override;
	virtual void NativeOnClicked() override;

	virtual void NativeOnUnhovered() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintImplementableEvent, Category = CommonButton, meta = (DisplayName = "On Right Clicked"))
	void BP_OnRightClicked();
private:
#pragma region BindWidgets
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional ,AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	
#pragma endregion
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	bool bUseUpperCaseForButtonText = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	FText ButtonDescriptionText;
	
protected:
	
	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = true, DisplayName = "On Right Clicked"))
	FOnRightButtonClicked OnButtonBaseRightClicked;
	
private:
	mutable FCommonButtonEvent OnRightClickedEvent;
	
	bool bRightButtonPressedOnButton = false;
};
