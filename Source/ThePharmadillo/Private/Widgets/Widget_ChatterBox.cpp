// Gondos Daniel all rights reserved.


#include "Widgets/Widget_ChatterBox.h"

#include "CommonRichTextBlock.h"
#include "DataAssets/GD_ChatterDataAsset.h"

void UWidget_ChatterBox::SetWidgetContent(UGD_ChatterDataAsset* ChatterDataAsset)
{
	CachedChatterDataContent = ChatterDataAsset;
	CurrentLineIndex = 0;

	if (NextLineTimeHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(NextLineTimeHandle);
	
	GetWorld()->GetTimerManager().SetTimer(
		NextLineTimeHandle,
		this,
		&ThisClass::ShowNextLine,
		MessageFrequency,
		true
	);
}

void UWidget_ChatterBox::ShowNextLine()
{
	if (CachedChatterDataContent->DialogueText.IsValidIndex(CurrentLineIndex))
	{
		CommonTextBlock_Chatter->SetText(CachedChatterDataContent->DialogueText[CurrentLineIndex++]);
		BlinkAnimation_BP();
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(NextLineTimeHandle); //When we ran out of lines we clear the timer
}
