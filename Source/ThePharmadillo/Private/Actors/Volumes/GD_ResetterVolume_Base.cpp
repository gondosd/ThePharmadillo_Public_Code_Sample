// Gondos Daniel all rights reserved.


#include "Actors/Volumes/GD_ResetterVolume_Base.h"
#include "Characters/GD_ThirdPersonCharacter.h"

void AGD_ResetterVolume_Base::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AGD_ThirdPersonCharacter* PlayerCharacter = Cast<AGD_ThirdPersonCharacter>(OtherActor))
		PlayerCharacter->ResetCharactersTransform();
	
	
}
