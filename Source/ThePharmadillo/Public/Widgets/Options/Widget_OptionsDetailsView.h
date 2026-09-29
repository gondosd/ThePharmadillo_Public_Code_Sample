// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class UScaleBox;
class UListDataObject_Base;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	virtual void NativeOnInitialized() override;

private:
#pragma region BoundWidgets
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Initiale;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScaleBox> ScaleBox_LazyImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> CommonRichText_Description;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> CommonRichText_DisabledReason;


#pragma endregion
};
