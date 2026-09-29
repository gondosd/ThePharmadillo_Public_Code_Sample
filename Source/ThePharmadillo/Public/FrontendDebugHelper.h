#pragma once

namespace Debug
{
	static void Print(const FString& Msg, const int32 InKey = -1, const float TimeToDisplay = 7.f, bool RandomColor = false, const FColor& InColor = FColor::Red) 
	{
		if (GEngine)
		{			
			GEngine->AddOnScreenDebugMessage(InKey, TimeToDisplay, RandomColor ? FColor::MakeRandomColor() : InColor, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
	
	
	static bool LineTraceSingleByProfileWithDebugLine(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, const FName ProfileName,const bool bShowDebugLine = false ,const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam)
	{
		const bool bHit = World->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, Params);
		if (bShowDebugLine)
			DrawDebugLine(World, Start,
						  bHit ? OutHit.ImpactPoint : End,
						  bHit ? FColor::Green : FColor::Red,
						  false,
						  2.0f,
						  0,
						  2.0f);
		
		return bHit;
	}
}