// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Widgets/Widget_ActivatableWidgetBase.h"
#include "Widget_CreditsScreen.generated.h"


class FCreditsScreenInputPreprocessor;
class UFrontendCommonBoundActionBar;
class UCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract)
class THEPHARMADILLO_API UWidget_CreditsScreen : public UWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
	
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SpeedUpKeyPressed(const FKey& Key);
	void AnyKeyPressed(const FKey& Key) const;
	void SpeedUpKeyReleased(const FKey& Key);
	
	UFUNCTION(BlueprintPure, meta = (HideSelfPin = "true"))
	FORCEINLINE float GetScrollingSpeed() const {return CurrentScrollingSpeed;};
	
private:
#pragma region Bindigns

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> CommonButton_SpeedUpCredits;
	
#pragma endregion
	
	void SpeedUpScrollingSpeed();
	void ResetScrollingSpeed();
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Scrolling Speed")
	float ScrollingBaseSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Scrolling Speed")
	float ScrollSpeedMultiplier = 10.f;
	
	float CurrentScrollingSpeed = ScrollingBaseSpeed;
	
	TSharedPtr<FCreditsScreenInputPreprocessor> CachedInputPreprocessor;
};
