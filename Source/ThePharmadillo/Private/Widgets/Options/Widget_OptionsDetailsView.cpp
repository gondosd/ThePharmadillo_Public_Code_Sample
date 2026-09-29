// Gondos Daniel all rights reserved.


#include "Widgets/Options/Widget_OptionsDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/ScaleBox.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	FString DisplayNameStr = InDataObject->GetDataDisplayName().ToString();
	CommonTextBlock_Initiale->SetText(FText::FromString(DisplayNameStr.Left(1)));
	CommonTextBlock_Title->SetText(FText::FromString(DisplayNameStr.RightChop(1)));
	
	TSoftObjectPtr<UTexture2D> ImageFound = InDataObject->GetSoftDescriptionImage();
	if (!ImageFound.IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(ImageFound, true);
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // HitTestInvisible is bugged. i cant set only for the scalebox
		ScaleBox_LazyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ScaleBox_LazyImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	CommonRichText_Description->SetText(InDataObject->GetDescriptionRichText());

	CommonRichText_DisabledReason->SetText(InDataObject->IsDataCurrentlyEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
}

void UWidget_OptionsDetailsView::ClearDetailsViewInfo()
{
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	CommonTextBlock_Initiale->SetText(FText::GetEmpty());
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	CommonRichText_Description->SetText(FText::GetEmpty());
	CommonRichText_DisabledReason->SetText(FText::GetEmpty());
}

void UWidget_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClearDetailsViewInfo();
}
