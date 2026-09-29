// Gondos Daniel all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AsyncAction.h"
#include "GD_K2Node_AsyncAction_Push_DialogueScreen.generated.h"

class UGD_DialogueDataAsset;
/**
 * 
 */
UCLASS()
class THEPHARMADILLOEDITOR_API UGD_K2Node_AsyncAction_Push_DialogueScreen : public UK2Node_AsyncAction
{
	GENERATED_BODY()
	
	UGD_K2Node_AsyncAction_Push_DialogueScreen();
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;


	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// Dynamic answer handling
	virtual void AllocateDefaultPins() override;
	virtual void PostPlacedNewNode() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	
	private:
	
	void SpawnSwitchOnName() const;
	// UPROPERTY(Transient)
	// TObjectPtr<UGD_DialogueDataAsset> CachedDialogueDataAsset;
};
