// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;
/**
 * 
 */
UCLASS()
class THEPHARMADILLO_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
	                                                   const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

private:
	UPROPERTY(EditAnywhere, Category = "Frontend List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
};
