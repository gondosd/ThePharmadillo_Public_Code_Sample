// Gondos Daniel all rights reserved.


#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag InTag)
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not find the widget stack ba the tag %s"), *InTag.ToString());
	return RegisteredWidgetStackMap.FindRef(InTag);
}

TArray<UCommonActivatableWidgetContainerBase*> UWidget_PrimaryLayout::GetAllRegisteredWidgetStacks() const
{
	TArray<UCommonActivatableWidgetContainerBase*> OutValues;
	RegisteredWidgetStackMap.GenerateValueArray(OutValues);
	return OutValues;
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
