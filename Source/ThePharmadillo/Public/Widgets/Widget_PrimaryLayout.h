// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class THEPHARMADILLO_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag InTag);
	TArray<UCommonActivatableWidgetContainerBase*> GetAllRegisteredWidgetStacks() const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
