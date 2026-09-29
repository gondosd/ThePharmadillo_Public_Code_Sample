// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonLazyImage;
class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_KeyRemapScreen : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
public:
	void SetDesiredInputType(ECommonInputType InDesiredInputType);
	void SetEquippedBindingIcon(const FSlateBrush& InBrush);
	
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);	
	void OnKeySelectCanceled(const FString& CanceledReason);
	
	
	//Delay a tick to make sure the input key is captured properly before calling the PreDeactivateCallback and deactivating the widget
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
	
#pragma region BoundWidgets

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;
	
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_EquippedBindingIcon;

#pragma endregion


	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;
	
	ECommonInputType CachedDesiredInputType;
	
};
