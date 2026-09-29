// Gondos Daniel all rights reserved.


#include "Widgets/Widget_DialogueScreen.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/ScrollBox.h"
#include "DataAssets/GD_DialogueDataAsset.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/FrontendHapticsSubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

static FString HARDSTOP_CHARACTERS = TEXT(".?!");
static float CONTROLLER_ANALOG_DEADZONE = 10.f;

UDialogueScreenInfoObject* UDialogueScreenInfoObject::CreateDialogueScreen(const UGD_DialogueDataAsset& InDialogueDataAsset)
{
	UDialogueScreenInfoObject* InfoObject = NewObject<UDialogueScreenInfoObject>();
	InfoObject->SpeakerName = InDialogueDataAsset.SpeakerName;
	InfoObject->SpeakerPicture = InDialogueDataAsset.SpeakerPicture;
	InfoObject->DialogueText = InDialogueDataAsset.DialogueText;
	InfoObject->AnswerOptions = InDialogueDataAsset.AnswerOptions;

	return InfoObject;
}

void UWidget_DialogueScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	OnWidgetClosed.AddWeakLambda(this, [this](UWidget_ActivatableWidgetBase*)
	{
		if (ClickedButtonCallback)
			ClickedButtonCallback(PendingAnswerID);
	});
}

void UWidget_DialogueScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	if (SkipAction)
	{
		SkipActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(SkipAction, true,
							  FSimpleDelegate::CreateUObject(
								  this, &ThisClass::FinishTypewriterEffect)));
	}
}

void UWidget_DialogueScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ContinueActionHandle.Unregister();
}

bool UWidget_DialogueScreen::NativeOnHandleBackAction()
{
	FinishTypewriterEffect();
	return Super::NativeOnHandleBackAction();
}


void UWidget_DialogueScreen::InitDialogueScreen(UDialogueScreenInfoObject* InScreenInfoObject, TFunction<void(FName)> InClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlock_SpeakerName && CommonLazyImage_SpeakerPicture && CommonTextBlock_DialogueText && DynamicEntryBox_Answers);

	//The message will be sent out on Closed.
	ClickedButtonCallback = MoveTemp(InClickedButtonCallback);
	PendingAnswerID = FName("Default");
	
	CommonTextBlock_SpeakerName->SetText(InScreenInfoObject->SpeakerName);
	CommonLazyImage_SpeakerPicture->SetBrushFromLazyTexture(InScreenInfoObject->SpeakerPicture);
	StartTypewriterEffect(InScreenInfoObject->DialogueText);

	//Checking if the entry box has old button created previously
	if (DynamicEntryBox_Answers->GetNumEntries() != 0)
	{
		/*
		 *Clearing the old buttons the entry box has. The widget type for the entry box 
		 *is specified in the child widget blueprint
		 */
		DynamicEntryBox_Answers->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			});
	}

	for (const TPair<FName, FText>& AvailableButtonInfo : InScreenInfoObject->AnswerOptions)
	{
		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Answers->CreateEntry<UFrontendCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.Value);
		AddedButton->OnClicked().AddLambda(
			[AvailableButtonInfo, this]()
			{
				PendingAnswerID = AvailableButtonInfo.Key;
				CloseWidget();
			});
	}

	ContinueUIActionBinding = FBindUIActionArgs(ContinueAction, true
	                                            , FSimpleDelegate::CreateLambda(
		                                            [this]()
		                                            {
			                                            CloseWidget();
		                                            }
	                                            ));
}

UWidget* UWidget_DialogueScreen::NativeGetDesiredFocusTarget() const
{
	InitializeAnswerOptionFocus();
	return Super::NativeGetDesiredFocusTarget();
}

FReply UWidget_DialogueScreen::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent)
{
	//Scrolling with Gamepad
	if (InAnalogEvent.GetKey() == EKeys::Gamepad_RightY || InAnalogEvent.GetKey() == EKeys::Gamepad_Right2D)
	{
		const float AnalogDiff = ControllerScrollSensitivity * InAnalogEvent.GetAnalogValue() * -1.f; //flips the scroll direction
		const float Offset = FMath::Clamp(ScrollBox_Dialogue->GetScrollOffset() + AnalogDiff, 0.f, ScrollBox_Dialogue->GetScrollOffsetOfEnd()); //no overscroll
		if (FMath::Abs(AnalogDiff) >= CONTROLLER_ANALOG_DEADZONE) //deadzoning
			ScrollBox_Dialogue->SetScrollOffset(Offset);

		return FReply::Handled();
	}

	return Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
}

void UWidget_DialogueScreen::StartTypewriterEffect(const FText& TextToDisplay)
{
	FullMessageToDisplay = TextToDisplay.ToString();
	DynamicEntryBox_Answers->SetVisibility(ESlateVisibility::Collapsed);
	CurrentCharIndex = 0;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
	                                       [this] { ShowNextLetter(); },
	                                       TypewriterEffectRate,
	                                       true);
}

void UWidget_DialogueScreen::ShowNextLetter()
{
	if (FullMessageToDisplay.IsValidIndex(CurrentCharIndex))
	{
		HandleTextDecoratorWords(); //rich text decorated words show up in one go
		DisplayDialogueText(FullMessageToDisplay.Left(CurrentCharIndex + 1));

		//Pauses a bit, if its the end of a sentence
		if (HARDSTOP_CHARACTERS.GetCharArray().Contains(FullMessageToDisplay[CurrentCharIndex]))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			                                       [this] { ShowNextLetter(); },
			                                       TypewriterEffectRate,
			                                       true,
			                                       PunctuationWaitTime);
		}
		CurrentCharIndex++;
	}
	else
	{
		FinishTypewriterEffect();
	}
}

void UWidget_DialogueScreen::HandleTextDecoratorWords()
{
	//This function checks for RichText decorators, and if there is any, it forces the CharIndex to jump to the end of it, so it wont look broken
	if (FullMessageToDisplay[CurrentCharIndex] == '<')
	{
		int ClosingSignCounter = 0;
		while (ClosingSignCounter < 2)
		{
			if (FullMessageToDisplay[CurrentCharIndex] == '>')
				ClosingSignCounter++;
			CurrentCharIndex++;
		}

		UFrontendHapticsSubsystem::Get(GetOwningLocalPlayer())->PlayDefaultWeakFeedbackEffect(ERumbleType::Menu);
		UGameplayStatics::PlaySound2D(this, DecoratorSound);
	}
}

void UWidget_DialogueScreen::DisplayDialogueText(const FString& TextToDisplay)
{
	CommonTextBlock_DialogueText->SetText(FText::FromString(TextToDisplay));
	UGameplayStatics::PlaySound2D(this, WritingSound);

	if (TextBlockCurrentSize != CommonTextBlock_DialogueText->GetDesiredSize())
	{
		TextBlockCurrentSize = CommonTextBlock_DialogueText->GetDesiredSize();
		ScrollBox_Dialogue->ScrollToEnd();
	}
}

void UWidget_DialogueScreen::InitializeAnswerOptionFocus() const
{
	if (DynamicEntryBox_Answers->GetNumEntries() != 0)
	{
		DynamicEntryBox_Answers->GetAllEntries()[0]->SetFocus();
	}
}

void UWidget_DialogueScreen::FinishTypewriterEffect()
{
	if (TimerHandle.IsValid())
	{
		DisplayDialogueText(FullMessageToDisplay);
		ScrollBox_Dialogue->ScrollToEnd();

		if (DynamicEntryBox_Answers->GetNumEntries() != 0)
		{
			DynamicEntryBox_Answers->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			if (ContinueUIActionBinding.IsSet())
				ContinueActionHandle = RegisterUIActionBinding(*ContinueUIActionBinding);
		}

		InitializeAnswerOptionFocus();

		SkipActionHandle.Unregister();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

