// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GD_Item_DataAsset.generated.h"

class UGD_GameplayEffect_Base;
/**
 * 
 */
UCLASS(Blueprintable)
class THEPHARMADILLO_API UGD_Item_DataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	TSoftClassPtr<UGD_GameplayEffect_Base> GameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	TSoftObjectPtr<USoundBase> InventoryConsumeSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	TSoftObjectPtr<USoundBase> InventoryAddSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = World)
	int32 HarvestCount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = World)
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = World)
	TArray<TSoftObjectPtr<UStaticMesh>> HarvestedMeshes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = World)
	TSoftObjectPtr<USoundBase> HarvestSound;
	
	
};
