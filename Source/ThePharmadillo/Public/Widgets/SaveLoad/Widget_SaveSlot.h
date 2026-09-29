// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widget_SaveSlot.generated.h"

class USaveSlot;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UWidget_SaveSlot : public UFrontendCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlotContent(USaveSlot* SaveSlot);
	
	UFUNCTION(BlueprintCallable)
	void DeleteSlotContent(USaveSlot* SlotToDelete);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeSlotContentDelete_BP();

protected:

	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
	
	UPROPERTY(BlueprintReadOnly)
	USaveSlot* CachedSaveSlot;
	
private:
	
#pragma region BindWidgets
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_TimeStamp;
	
	UPROPERTY(EditAnywhere ,BlueprintReadOnly, meta = (BindWidget ,AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_SavePreview;
	
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, Category = "Save Load", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UInputAction* DeleteSlotAction;

	FUIActionBindingHandle DeleteSlotActionHandle;
};
