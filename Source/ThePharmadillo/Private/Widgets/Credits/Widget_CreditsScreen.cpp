// Gondos Daniel all rights reserved.


#include "Widgets/Credits/Widget_CreditsScreen.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Framework/Application/IInputProcessor.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Input/CommonBoundActionBar.h"
#include "Subsystems/FrontendHapticsSubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"


class FCreditsScreenInputPreprocessor : public IInputProcessor
{
public:
	FCreditsScreenInputPreprocessor(
		TWeakObjectPtr<UInputMappingContext> InInputMapping,
		TWeakObjectPtr<UInputAction> InTriggeringEnhancedInputAction)
		: CachedInputMapping(InInputMapping), CachedTriggeringEnhancedInputAction(InTriggeringEnhancedInputAction)
	{}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override{}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorSpeedUpKeyPressedDelegate, const FKey& /*PressedKey*/);
	FOnInputPreProcessorSpeedUpKeyPressedDelegate OnInputPreProcessorSpeedUpKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorAnyKeyPressedDelegate, const FKey& /*AnyKeyPressed*/);
	FOnInputPreProcessorAnyKeyPressedDelegate OnInputPreProcessorAnyKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorSpeedUpKeyReleasedDelegate, const FKey& /*Released*/);
	FOnInputPreProcessorSpeedUpKeyReleasedDelegate OnInputPreProcessorSpeedUpKeyReleased;

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessKey(InKeyEvent.GetKey(), true);
		return false;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		//default confirm on gamepad is being simulated as right click in InPressedKey, this is the fix
		ProcessKey(MouseEvent.GetEffectingButton(), true);
		return false;
	}

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessKey(InKeyEvent.GetKey(), false);
		return false;
	}

protected:
	void ProcessKey(const FKey& InPressedKey, const bool bIsDown) const
	{
		if (CachedInputMapping.IsValid())
		{
			for (FEnhancedActionKeyMapping Element : CachedInputMapping.Get()->GetMappings())
			{
				if (Element.Action == CachedTriggeringEnhancedInputAction)
				{
					if (Element.Key == InPressedKey)
					{
						if (bIsDown)
						{
							OnInputPreProcessorSpeedUpKeyPressed.ExecuteIfBound(InPressedKey);
						}
						else
						{
							OnInputPreProcessorSpeedUpKeyReleased.ExecuteIfBound(InPressedKey);
						}
					}
				}
			}
		}

		if (bIsDown)
			OnInputPreProcessorAnyKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	TWeakObjectPtr<UInputMappingContext> CachedInputMapping;
	TWeakObjectPtr<UInputAction> CachedTriggeringEnhancedInputAction;
};

void UWidget_CreditsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_SpeedUpCredits->OnPressed().AddUObject(this, &ThisClass::SpeedUpScrollingSpeed);
	CommonButton_SpeedUpCredits->OnReleased().AddUObject(this, &ThisClass::ResetScrollingSpeed);
}

void UWidget_CreditsScreen::NativeOnActivated()
{
	CommonButton_SpeedUpCredits->SetVisibility(ESlateVisibility::Collapsed);

	ResetScrollingSpeed();

	if (InputMapping)
	{
		CachedInputPreprocessor = MakeShared<FCreditsScreenInputPreprocessor>(InputMapping, CommonButton_SpeedUpCredits->TriggeringEnhancedInputAction);
		CachedInputPreprocessor->OnInputPreProcessorSpeedUpKeyPressed.BindUObject(this, &ThisClass::SpeedUpKeyPressed);
		CachedInputPreprocessor->OnInputPreProcessorAnyKeyPressed.BindUObject(this, &ThisClass::AnyKeyPressed);
		CachedInputPreprocessor->OnInputPreProcessorSpeedUpKeyReleased.BindUObject(this, &ThisClass::SpeedUpKeyReleased);

		FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);
	}

	Super::NativeOnActivated();
}

void UWidget_CreditsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);
		CachedInputPreprocessor.Reset();
	}
}

FReply UWidget_CreditsScreen::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetCursorDelta().ComponentwiseAllGreaterThan(FDeprecateSlateVector2D(2.f, 2.f)))
		CommonButton_SpeedUpCredits->SetVisibility(ESlateVisibility::Visible);
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UWidget_CreditsScreen::SpeedUpKeyPressed(const FKey& Key)
{
	SpeedUpScrollingSpeed();
	UFrontendHapticsSubsystem::Get(GetOwningLocalPlayer())->PlayDefaultWeakFeedbackEffect(ERumbleType::Menu);
}

void UWidget_CreditsScreen::AnyKeyPressed(const FKey& Key) const
{
	if (CommonButton_SpeedUpCredits->GetVisibility() != ESlateVisibility::Visible)
		CommonButton_SpeedUpCredits->SetVisibility(ESlateVisibility::Visible);
}

void UWidget_CreditsScreen::SpeedUpKeyReleased(const FKey& Key)
{
	ResetScrollingSpeed();
}

void UWidget_CreditsScreen::SpeedUpScrollingSpeed()
{
	CurrentScrollingSpeed = ScrollingBaseSpeed * ScrollSpeedMultiplier;
}

void UWidget_CreditsScreen::ResetScrollingSpeed()
{
	CurrentScrollingSpeed = ScrollingBaseSpeed;
}
