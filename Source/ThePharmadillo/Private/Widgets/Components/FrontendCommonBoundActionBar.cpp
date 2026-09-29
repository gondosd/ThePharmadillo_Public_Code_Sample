// Gondos Daniel all rights reserved.


#include "Widgets/Components/FrontendCommonBoundActionBar.h"


void UFrontendCommonBoundActionBar::ActionBarUpdateEndImpl()
{
	OnVisualUpdated.Broadcast();
	Super::ActionBarUpdateEndImpl();
}
