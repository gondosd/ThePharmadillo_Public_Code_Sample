// Gondos Daniel all rights reserved.


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in the project settings as loading screen "));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();
	
	return LoadedLoadingScreenWidget;
}
