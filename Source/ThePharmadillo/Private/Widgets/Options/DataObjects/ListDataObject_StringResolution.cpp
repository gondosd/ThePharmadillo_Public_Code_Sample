// Gondos Daniel all rights reserved.


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"

#include "FrontendDebugHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);


	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);

	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText =ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	//Resolution Value from Dynamic Getter: : (X=1920,Y=1080)
	return FString::Printf(TEXT("(X=%i, Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	return FText::FromString(FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y));
}
