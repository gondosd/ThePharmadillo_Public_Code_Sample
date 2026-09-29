// Gondos Daniel all rights reserved.


#include "Widgets/Options/Widget_KeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonUITypes.h"
#include "EnhancedInputSubsystems.h"
#include "FrontendDebugHelper.h"
#include "ICommonInputModule.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(TWeakObjectPtr<ULocalPlayer> InLocalPlayer) : CachedWeakOwningLocalPlayer(InLocalPlayer)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /*PressedKey*/);
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectedCanceledDelegate, const FString& /*CanceledReason*/);
	FOnInputPreProcessorKeySelectedCanceledDelegate OnInputPreProcessorKeySelectedCanceled;

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}

protected:
	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Key Remap has been canceled"));
			return;
		}
		const ULocalPlayer* LocalPlayer = CachedWeakOwningLocalPlayer.Get();
		check(LocalPlayer);

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
		check(CommonInputSubsystem);

		UEnhancedInputLocalPlayerSubsystem* EISubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(EISubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		if ((CurrentInputType == ECommonInputType::Gamepad) && (InPressedKey == EKeys::LeftMouseButton))
		//default confirm on gamepad is being simulated as right click in InPressedKey, this is the fix
		{
			UInputAction* DefaultAccept = ICommonInputModule::GetSettings().GetEnhancedInputClickAction();
			TArray<FKey> BoundKeys = EISubsystem->QueryKeysMappedToAction(DefaultAccept);

			FKey GamepadKey = EKeys::Invalid;
			for (const FKey& Key : BoundKeys)
			{
				if (Key.IsGamepadKey())
				{
					GamepadKey = Key;
					break;
				}
			}
			check(GamepadKey.IsValid())
			OnInputPreProcessorKeyPressed.ExecuteIfBound(GamepadKey);
			return;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};

void UWidget_KeyRemapScreen::SetDesiredInputType(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::SetEquippedBindingIcon(const FSlateBrush& InBrush)
{
	CommonLazyImage_EquippedBindingIcon->SetBrush(InBrush);
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(GetOwningLocalPlayer());
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectedCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FString InputDeviceName;
	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse & Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default: ;
	}

	const FString DisplayRichMessage = FString::Printf(
		TEXT("<KeyRemap_Default>Press any</> <KeyRemap_Highlight>%s</> <KeyRemap_Default> key.</>\n"
			"<KeyRemap_Default>The currently equipped button is:</>"), *InputDeviceName
	);
	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);
		CachedInputPreprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget([this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget([this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	//Delay a tick to make sure the input is processed correctly

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([PreDeactivateCallback, this](float DeltaTime)
			{
				PreDeactivateCallback();
				DeactivateWidget();
				return false; //to stop the ticking
			}
		));
}
