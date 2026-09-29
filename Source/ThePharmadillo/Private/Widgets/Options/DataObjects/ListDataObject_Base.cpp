// Gondos Daniel all rights reserved.


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionsDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &UListDataObject_Base::OnEditDependencyModified);
	}
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;
	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;
	for (const FOptionsDataEditConditionsDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}
		bIsEditable = false;
		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());
		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();
			//If the current value this data object has can be set to the forced value
			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}
	return bIsEditable;
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifiedReason ModifiedReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifiedReason);
	if (bShouldApplyChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}

void UListDataObject_Base::OnEditDependencyModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifiedReason ModifiedReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifiedReason);
}
