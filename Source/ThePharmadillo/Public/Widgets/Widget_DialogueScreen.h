// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ActivatableWidgetBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Widget_DialogueScreen.generated.h"


class UScrollBox;
class UCommonRichTextBlock;
class UInputAction;
class UCommonLazyImage;
class UDynamicEntryBox;
class UConfirmScreenInfoObject;
class UGD_DialogueDataAsset;

UCLASS()
class THEPHARMADILLO_API UDialogueScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UDialogueScreenInfoObject* CreateDialogueScreen(const UGD_DialogueDataAsset& InDialogueDataAsset);
	
	
	UPROPERTY(Transient)
	FText SpeakerName;
	
	UPROPERTY(Transient)
	TSoftObjectPtr<UTexture2D> SpeakerPicture;
	
	UPROPERTY(Transient)
	FText DialogueText;
	
	UPROPERTY(Transient)
	TMap<FName , FText> AnswerOptions;
	
};

UCLASS(Abstract)
class THEPHARMADILLO_API UWidget_DialogueScreen : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	//Gets called outside the class when this widget is constructed and before its pushed to the  modal stack
	void InitDialogueScreen(UDialogueScreenInfoObject* InScreenInfoObject, TFunction<void(FName)> InClickedButtonCallback);
	UFUNCTION(BlueprintCallable)
	void FinishTypewriterEffect();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual bool NativeOnHandleBackAction() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;

	UPROPERTY(EditDefaultsOnly, Category = TypewriterEffect);
	float TypewriterEffectRate = 0.05f;
	UPROPERTY(EditDefaultsOnly, Category = TypewriterEffect);
	float PunctuationWaitTime = 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = TypewriterEffect);
	TObjectPtr<USoundBase> WritingSound;
	UPROPERTY(EditDefaultsOnly, Category = TypewriterEffect);
	TObjectPtr<USoundBase> DecoratorSound;
	UPROPERTY(EditDefaultsOnly, Category = TypewriterEffect);
	float ControllerScrollSensitivity = 20.f;

private:
	void StartTypewriterEffect(const FText& TextToDisplay);
	void ShowNextLetter();
	void HandleTextDecoratorWords();
	void DisplayDialogueText(const FString& TextToDisplay);
	
	void InitializeAnswerOptionFocus() const;
	
	FTimerHandle TimerHandle;
	int32 CurrentCharIndex = 0;
	FString FullMessageToDisplay;
	FVector2D TextBlockCurrentSize;
	
	//This Callback is being called whenever the dialogue screen is closed. With the Pending aswer setted from InitDialogueScreen();
	TFunction<void(FName)> ClickedButtonCallback;
	FName PendingAnswerID = FName("Default");	
	
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UInputAction* SkipAction;
	FUIActionBindingHandle SkipActionHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UInputAction* ContinueAction;
	FUIActionBindingHandle ContinueActionHandle;
	TOptional<FBindUIActionArgs> ContinueUIActionBinding;
	
#pragma region Bindigns
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonTextBlock_SpeakerName;
	
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_SpeakerPicture;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonTextBlock_DialogueText;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox_Dialogue;

	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Answers;
#pragma endregion
};
