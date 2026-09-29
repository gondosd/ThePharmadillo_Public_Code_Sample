// Gondos Daniel all rights reserved.


#include "Widgets/SaveLoad/Widget_SaveSlot.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "FrontendDebugHelper.h"
#include "PharmadilloFunctionLibrary.h"
#include "SaveSlot.h"
#include "Input/CommonUIInputTypes.h"

void UWidget_SaveSlot::UpdateSlotContent(USaveSlot* SaveSlot)
{
	CachedSaveSlot = SaveSlot;
	
	if (CachedSaveSlot)
	{
		
		CommonLazyImage_SavePreview->SetVisibility(ESlateVisibility::HitTestInvisible);
		CommonLazyImage_SavePreview->SetBrushFromTexture(CachedSaveSlot->Thumbnail);
	
		//Area name
		SetButtonText(CachedSaveSlot->DisplayName);
		SetButtonDescriptionText(FText::Format(FText::FromString("Continue your journey in the {0} area"), CachedSaveSlot->DisplayName));
	
		//Timestamp
		FNumberFormattingOptions YearOptions;
		YearOptions.MinimumIntegralDigits = 4;
		YearOptions.SetUseGrouping(false);
		FNumberFormattingOptions Options;
		Options.MinimumIntegralDigits = 2;
		
		const FText FinalTimestamp = FText::Format(
			FText::FromString("{0}/{1}/{2} - {3}:{4}"),
			FText::AsNumber(CachedSaveSlot->Stats.SaveDate.GetYear(), &YearOptions),
			FText::AsNumber(CachedSaveSlot->Stats.SaveDate.GetMonth(), &Options),
			FText::AsNumber(CachedSaveSlot->Stats.SaveDate.GetDay(), &Options),
			FText::AsNumber(CachedSaveSlot->Stats.SaveDate.GetHour(), &Options),
			FText::AsNumber(CachedSaveSlot->Stats.SaveDate.GetMinute(), &Options)
		);
		
		CommonTextBlock_TimeStamp->SetText(FinalTimestamp);
	}
	else //this resets to clean visual
	{
		CommonLazyImage_SavePreview->SetVisibility(ESlateVisibility::Collapsed);
		SetButtonText(FText::FromString(TEXT("Start New Game")));
		SetButtonDescriptionText(FText::FromString(TEXT("A new journey awaits")));
		CommonTextBlock_TimeStamp->SetText(FText::GetEmpty());
	}
}

void UWidget_SaveSlot::DeleteSlotContent(USaveSlot* SlotToDelete)
{
	UPharmadilloFunctionLibrary::DeleteGameBySlot(this, SlotToDelete);

	UpdateSlotContent(nullptr);
	
	if (DeleteSlotActionHandle.IsValid())
		DeleteSlotActionHandle.Unregister();
}

void UWidget_SaveSlot::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
	
	if (DeleteSlotAction && CachedSaveSlot) //  if slot is empty this shouldnt appear
	{
		DeleteSlotActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(DeleteSlotAction, true,
							  FSimpleDelegate::CreateUObject(this, &ThisClass::InitializeSlotContentDelete_BP)));
	}
	
}

void UWidget_SaveSlot::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
	
	if (DeleteSlotActionHandle.IsValid())
		DeleteSlotActionHandle.Unregister();
}

