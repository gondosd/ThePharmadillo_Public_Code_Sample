// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Widget_ChatterBox.generated.h"


class UGD_ChatterDataAsset;
class UCommonRichTextBlock;

UCLASS(Abstract)
class THEPHARMADILLO_API UWidget_ChatterBox : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetWidgetContent(UGD_ChatterDataAsset* ChatterDataAsset);

protected:
	//Show-up animation should be hooked on this
	UFUNCTION(BlueprintImplementableEvent)
	void BlinkAnimation_BP();

private:
	void ShowNextLine();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MessageFrequency = 3.f;

private:
	UPROPERTY(Transient)
	TObjectPtr<UGD_ChatterDataAsset> CachedChatterDataContent;
	int32 CurrentLineIndex = 0;
	
	FTimerHandle NextLineTimeHandle;


#pragma region Bindigns
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonTextBlock_Chatter;


#pragma endregion
};
