// Gondos Daniel all rights reserved.


#include "Subsystems/GD_LevelTransitionSubsystem.h"

#include "Characters/GD_ThirdPersonCharacter.h"
#include "Characters/CharacterComponents/GD_InventoryComponent.h"
#include "FrameworkClasses/GD_GameplayPlayerController.h"
#include "FrameworkClasses/GD_PlayerStateBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


void UGD_LevelTransitionSubsystem::ForEachSaveGameProperty(const UClass* Class, TFunctionRef<void(FProperty*)> Func)
{
	if (!Class) return;

	for (TFieldIterator<FProperty> PropIt(Class); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (Property && Property->HasAnyPropertyFlags(CPF_SaveGame))
		{
			Func(Property);
		}
	}
}


UGD_LevelTransitionSubsystem* UGD_LevelTransitionSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UGD_LevelTransitionSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UGD_LevelTransitionSubsystem::CacheClassesForTravel()
{
	// Cache every class here which is necessary for a proper level transition but only between game levels
	const auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
	
	//if its the main menu, no caching
	if (!PC->IsA(AGD_GameplayPlayerController::StaticClass()))
	{
		bHasCachedData = false;
		return;
	}
	const auto InventoryComponent = PC->GetPlayerState<AGD_PlayerStateBase>()->GetInventoryComponent();
	if (!InventoryComponent) return;

	const auto PlayerCharacter = Cast<AGD_ThirdPersonCharacter>(PC->GetCharacter());
	if (!PlayerCharacter) return;

	CapturedPCData = CaptureActor(PC);
	CapturedInventoryData = CaptureActor(InventoryComponent);
	CapturedPlacerCharacterData = CaptureActor(PlayerCharacter);

	bHasCachedData = true;
}

void UGD_LevelTransitionSubsystem::LoadCachedDataAfterTravel(bool bClearAfterLoad)
{
	if (!bHasCachedData) return;

	//load only if its a gameplay level
	const auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	const auto InventoryComponent = PC->GetPlayerState<AGD_PlayerStateBase>()->GetInventoryComponent();
	if (!InventoryComponent) return; 

	const auto PlayerCharacter =Cast<AGD_ThirdPersonCharacter>( PC->GetCharacter());
	if (!PlayerCharacter) return;

	RestoreSingleActor(PC, CapturedPCData);
	RestoreSingleActor(InventoryComponent, CapturedInventoryData);
	RestoreSingleActor(PlayerCharacter, CapturedPlacerCharacterData);

	if (bClearAfterLoad)
	{
		bHasCachedData = false;
	}
}

FGD_CachedActorData UGD_LevelTransitionSubsystem::CaptureActor(UObject* Object)
{
	if (!Object) return FGD_CachedActorData();

	FGD_CachedActorData Result;
	Result.ActorClass = Object->GetClass();

	ForEachSaveGameProperty(Object->GetClass(), [&](FProperty* Property)
	{
		FGD_CachedProperty Entry;
		Entry.PropertyName = Property->GetFName();

		const int32 PropSize = Property->GetSize() * Property->ArrayDim;
		Entry.RawBytes.SetNumZeroed(PropSize);

		const void* SrcValuePtr = Property->ContainerPtrToValuePtr<void>(Object);
		Property->CopyCompleteValue(Entry.RawBytes.GetData(), SrcValuePtr);

		CollectObjectRefs(Property, SrcValuePtr, Result.ReferencedObjects);
		
		// if (const FObjectProperty* ObjProp = CastField<FObjectProperty>(Property))
		// {
		// 	if (UObject* Referenced = ObjProp->GetObjectPropertyValue(SrcValuePtr))
		// 	{
		// 		Result.ReferencedObjects.AddUnique(Referenced);
		// 	}
		// }
		// else if (const FArrayProperty* ArrProp = CastField<FArrayProperty>(Property))
		// {
		// 	if (const FObjectProperty* InnerObjProp = CastField<FObjectProperty>(ArrProp->Inner))
		// 	{
		// 		FScriptArrayHelper Helper(ArrProp, SrcValuePtr);
		// 		for (int32 i = 0; i < Helper.Num(); ++i)
		// 		{
		// 			if (UObject* Referenced = InnerObjProp->GetObjectPropertyValue(Helper.GetRawPtr(i)))
		// 			{
		// 				Result.ReferencedObjects.AddUnique(Referenced);
		// 			}
		// 		}
		// 	}
		// }

		Result.Properties.Add(MoveTemp(Entry));
	});

	return Result;
}

int32 UGD_LevelTransitionSubsystem::RestoreSingleActor(UObject* Object, const FGD_CachedActorData& CachedData)
{
	TMap<FName, FProperty*> CurrentProps;
	ForEachSaveGameProperty(Object->GetClass(), [&](FProperty* Property)
	{
		CurrentProps.Add(Property->GetFName(), Property);
	});

	int32 RestoredCount = 0;

	for (const FGD_CachedProperty& Entry : CachedData.Properties)
	{
		FProperty** MatchedProp = CurrentProps.Find(Entry.PropertyName);
		if (!MatchedProp)
		{
			continue;
		}

		FProperty* TargetProp = *MatchedProp;

		const int32 ExpectedSize = TargetProp->GetSize() * TargetProp->ArrayDim;
		if (ExpectedSize != Entry.RawBytes.Num())
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("UGD_LevelTransitionSubsystem: Size/type mismatch for property '%s' on actor '%s' (expected %d bytes, cached %d) -- skipped."),
			       *Entry.PropertyName.ToString(), *Object->GetName(), ExpectedSize, Entry.RawBytes.Num());
			continue;
		}

		void* DestValuePtr = TargetProp->ContainerPtrToValuePtr<void>(Object);
		TargetProp->CopyCompleteValue(DestValuePtr, Entry.RawBytes.GetData());
		++RestoredCount;
	}

	return RestoredCount;
}

void UGD_LevelTransitionSubsystem::CollectObjectRefs(FProperty* Property, const void* ValuePtr, TArray<TObjectPtr<UObject>>& OutRefs)
{
	if (const FObjectProperty* ObjProp = CastField<FObjectProperty>(Property))
	{
		if (UObject* Referenced = ObjProp->GetObjectPropertyValue(ValuePtr))
		{
			OutRefs.AddUnique(Referenced);
		}
	}
	else if (const FStructProperty* StructProp = CastField<FStructProperty>(Property))
	{
		for (TFieldIterator<FProperty> It(StructProp->Struct); It; ++It)
		{
			CollectObjectRefs(*It, It->ContainerPtrToValuePtr<void>(ValuePtr), OutRefs);
		}
	}
	else if (const FArrayProperty* ArrProp = CastField<FArrayProperty>(Property))
	{
		FScriptArrayHelper Helper(ArrProp, ValuePtr);
		for (int32 i = 0; i < Helper.Num(); ++i)
		{
			CollectObjectRefs(ArrProp->Inner, Helper.GetRawPtr(i), OutRefs);
		}
	}
}
