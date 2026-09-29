// Gondos Daniel all rights reserved.


#include "Widgets/Components/FrontendCommonBoundActionButton.h"
#include "Subsystems/FrontendHapticsSubsystem.h"

void UFrontendCommonBoundActionButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	UFrontendHapticsSubsystem::Get(GetOwningLocalPlayer())->PlayDefaultWeakFeedbackEffect(ERumbleType::Menu);
}
