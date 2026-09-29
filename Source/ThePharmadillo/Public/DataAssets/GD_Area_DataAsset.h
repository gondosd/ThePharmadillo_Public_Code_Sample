// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GD_Area_DataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class THEPHARMADILLO_API UGD_Area_DataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	//This is whats being displayed on the top of the shield e.g.: "Welcome To"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ShieldVisuals)
	FText PreText = FText::FromString(TEXT("Welcome To"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ShieldVisuals)
	FText AreaName = FText::FromString(TEXT("Unnamed Area"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ShieldVisuals)
	TSoftObjectPtr<UTexture2D> AreaIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AreaAmbients)
	TSoftObjectPtr<USoundBase> AreaMusic;

	//Area Size is for prioritizing the volumes when there is an overlap. the smaller ground coverage have bigger priority 
	UPROPERTY()
	float AreaSize = 0.0f;

	//Necessary so overlapping same name volumes can be handled as one area
	UPROPERTY()
	FVector VolumeLocation = FVector();
	
	friend bool operator<(const UGD_Area_DataAsset& Lhs, const UGD_Area_DataAsset& RHS) { return Lhs.AreaSize < RHS.AreaSize; }
	friend bool operator<=(const UGD_Area_DataAsset& Lhs, const UGD_Area_DataAsset& RHS) { return !(RHS < Lhs); }
	friend bool operator>(const UGD_Area_DataAsset& Lhs, const UGD_Area_DataAsset& RHS) { return RHS < Lhs; }
	friend bool operator>=(const UGD_Area_DataAsset& Lhs, const UGD_Area_DataAsset& RHS) { return !(Lhs < RHS); }
};
