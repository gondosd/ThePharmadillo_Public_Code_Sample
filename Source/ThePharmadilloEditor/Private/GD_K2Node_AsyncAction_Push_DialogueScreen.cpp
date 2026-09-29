// Gondos Daniel all rights reserved.


#include "GD_K2Node_AsyncAction_Push_DialogueScreen.h"

// #include "BlueprintActionDatabaseRegistrar.h"
// #include "BlueprintNodeSpawner.h"
// #include "FrontendDebugHelper.h"
// #include "K2Node_ExecutionSequence.h"
// #include "K2Node_SwitchName.h"
// #include "KismetCompiler.h"
#include "EdGraphSchema_K2_Actions.h"
#include "K2Node_SwitchName.h"
#include "AsyncActions/GD_AsyncAction_Push_DialogueScreen.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet2/BlueprintEditorUtils.h"


static const FName DIALOGUE_ASSET_PIN_NAME(TEXT("DialogueDataAsset"));


#define LOCTEXT_NAMESPACE "UGD_K2Node_AsyncAction_Push_DialogueScreen"

UGD_K2Node_AsyncAction_Push_DialogueScreen::UGD_K2Node_AsyncAction_Push_DialogueScreen()
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UGD_AsyncAction_Push_DialogueScreen, PushDialogueScreen);
	ProxyFactoryClass = UGD_AsyncAction_Push_DialogueScreen::StaticClass();
	ProxyClass = UGD_AsyncAction_Push_DialogueScreen::StaticClass();
}

FText UGD_K2Node_AsyncAction_Push_DialogueScreen::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (UEdGraphPin* AssetPin = FindPin(DIALOGUE_ASSET_PIN_NAME))
	{
		if (AssetPin->DefaultObject)
		{
			if (UGD_DialogueDataAsset* Data = Cast<UGD_DialogueDataAsset>(AssetPin->DefaultObject))
			{
				return FText::Format(FText::FromString("{0} \n Dialogue ID: {1}"), Super::GetNodeTitle(TitleType), FText::FromName(Data->DialogueID));
			}
		}
		else if (AssetPin->LinkedTo.Num() > 0)
		{
			return FText::FromString("ID: Dynamic (Linked)");
		}
	}
	return Super::GetNodeTitle(TitleType);
}

FText UGD_K2Node_AsyncAction_Push_DialogueScreen::GetTooltipText() const
{
	if (UEdGraphPin* AssetPin = FindPin(DIALOGUE_ASSET_PIN_NAME))
	{
		if (AssetPin->DefaultObject)
		{
			if (const UGD_DialogueDataAsset* Data = Cast<UGD_DialogueDataAsset>(AssetPin->DefaultObject))
			{
				return FText::Format(INVTEXT("Dialogue Text:\n{0}"), Data->DialogueText);
			}
		}
	}

	return Super::GetTooltipText();
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	if (Pin->PinName == DIALOGUE_ASSET_PIN_NAME)
	{
		//CachedDialogueDataAsset = Cast<UGD_DialogueDataAsset>(Pin->DefaultObject);
		ReconstructNode();
	}
}

FLinearColor UGD_K2Node_AsyncAction_Push_DialogueScreen::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 0.5f, 0.5f);
}

FText UGD_K2Node_AsyncAction_Push_DialogueScreen::GetMenuCategory() const
{
	return FText::FromString("Game|Dialogue");
}

bool UGD_K2Node_AsyncAction_Push_DialogueScreen::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	return Super::IsCompatibleWithGraph(TargetGraph);
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(("Dialogue"), LOCTEXT("Dialogue", "Dialogue Actions"));

	Section.AddMenuEntry(
		"CreateSwitchOnName",
		LOCTEXT("CreateSwitch", "Create Switch On Name"),
		LOCTEXT("SpawnNode", "Spawns a Switch on Name node pre-filled with output pins."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateUObject(this, &ThisClass::SpawnSwitchOnName))
	);
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	//TODO: finish this. Pins arent staying permanently on load. but from changeing its fine

	// if (CachedDialogueDataAsset)
	// {
	// 	if (const UGD_DialogueDataAsset* Data = Cast<UGD_DialogueDataAsset>(CachedDialogueDataAsset))
	// 	{
	// 		for (const TPair<FName, FText>& Answer : Data->AnswerOptions)
	// 		{
	// 			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, Answer.Key);
	// 		}
	// 	}
	// }
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	//TODO: God knows why this is not working. the custom added pins arent firing. needs fixing so no switch on name node will be necessary everywhere

	/*
	bool bIsErrorFree = true;
	
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	
	// K2 node output pins (Super should already bind the internal async function to these)
	UEdGraphPin* InternalAnswerIDPin = FindPin(TEXT("AnswerID"));
	UEdGraphPin* InternalExecPin = FindPin(TEXT("OnButtonClicked"));
	
	// built-in switch on FName node
	UK2Node_SwitchName* SwitchNode = CompilerContext.SpawnIntermediateNode<UK2Node_SwitchName>(this, SourceGraph);
	SwitchNode->bHasDefaultPin = false;
	SwitchNode->AllocateDefaultPins();
		
	// noderegister so its included in the build - dunno if its really necessary
	CompilerContext.MessageLog.NotifyIntermediateObjectCreation(SwitchNode, this);
	
	// async action outputs to the switch on FName input
	bIsErrorFree &= Schema->TryCreateConnection(InternalExecPin, SwitchNode->GetExecPin());
	bIsErrorFree &= Schema->TryCreateConnection(InternalAnswerIDPin, SwitchNode->GetSelectionPin());
	
	
	// dynamic pin mappings
	for (int32 PinIdx = 0; PinIdx < Pins.Num(); PinIdx++)
	{
		UEdGraphPin* CurrentPin = Pins[PinIdx];
	
		// filter for our custom pins
		if (CurrentPin->Direction == EGPD_Output &&
			CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec &&
			CurrentPin->PinName != TEXT("OnButtonClicked") &&
			CurrentPin->PinName != UEdGraphSchema_K2::PN_Then)
		{
			//add new pin for the switch with the correct name
			SwitchNode->AddPinToSwitchNode();
			UEdGraphPin* NewSwitchPin = SwitchNode->Pins.Last();
			NewSwitchPin->PinName = CurrentPin->PinName;
	
			// bind the custom output pins to the switch output pins
			bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *NewSwitchPin).CanSafeConnect();

		}
	}
	
	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "BaseAsyncTask: Internal connection error. @@").ToString(), this);
	}

	// 6. Break any remaining links on the original node to finalize expansion
	BreakAllNodeLinks();*/
}

void UGD_K2Node_AsyncAction_Push_DialogueScreen::SpawnSwitchOnName() const
{
	UEdGraph* Graph = GetGraph();

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	FVector2D SpawnPos(NodePosX + 400, NodePosY + 50);

	UK2Node_SwitchName* SwitchNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_SwitchName>(Graph, SpawnPos, EK2NewNodeFlags::SelectNewNode);

	if (SwitchNode)
	{
		if (UEdGraphPin* AssetPin = FindPin(DIALOGUE_ASSET_PIN_NAME))
		{
			if (AssetPin->DefaultObject)
			{
				if (UGD_DialogueDataAsset* Data = Cast<UGD_DialogueDataAsset>(AssetPin->DefaultObject))
				{
					for (TPair<FName, FText> AnswerOption : Data->AnswerOptions)
					{
						SwitchNode->PinNames.Add(AnswerOption.Key);
						SwitchNode->CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, SwitchNode->PinNames.Last());
					}
				}
			}

			UEdGraphPin* NamePin = FindPin(FName("AnswerID"));
			UEdGraphPin* SwitchInputPin = SwitchNode->GetSelectionPin();

			UEdGraphPin* InternalExecPin = FindPin(FName("OnButtonClicked"));
			UEdGraphPin* SwitchExecPin = SwitchNode->GetExecPin();

			if (NamePin && SwitchInputPin && InternalExecPin && SwitchExecPin)
			{
				K2Schema->TryCreateConnection(NamePin, SwitchInputPin);
				K2Schema->TryCreateConnection(InternalExecPin, SwitchExecPin);
			}
			
			Graph->NotifyGraphChanged();
			FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
		}
	}
}
#undef LOCTEXT_NAMESPACE
