// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "ListDataObject_Base.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UFrontendEnhancedInputUserSettings;
class UCommonInputSubsystem;
class UEnhancedPlayerMappableKeyProfile;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings,
	                      UEnhancedPlayerMappableKeyProfile* InKeyProfile,
	                      const FPlayerKeyMapping& InOwningPlayerKeyboardKeyMapping, const FPlayerKeyMapping& InOwningPlayerGamepadKeyMapping);
	
	FSlateBrush GetIconFromCurrentKey(ECommonInputType InputType) const;
	
	void BindNewInputKey(const FKey& InNewKey, ECommonInputType CurrentInputType);

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	EPlayerMappableKeySlot GetSlotFromCurrentInputType() const;

private:
	
	FPlayerKeyMapping* GetOwningKeyMapping() const;
	
	
	UPROPERTY(Transient)
	TObjectPtr<UCommonInputSubsystem> CommonInputSubsystem;
	
	UPROPERTY(Transient)
	UFrontendEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeyboardKeySlot;
	EPlayerMappableKeySlot CachedOwningMappableGamepadKeySlot;
};
