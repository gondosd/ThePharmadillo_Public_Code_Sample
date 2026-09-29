// Gondos Daniel all rights reserved.


#include "Widgets/Options/OptionsDataRegistry.h"

#include "EnhancedInputSubsystems.h"
#include "PharmadilloFunctionLibrary.h"
#include "FrontendDebugHelper.h"
#include "GD_GameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"


#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName)\
		MakeShared<FOptionsDataInteractionHelper>( GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey)\
		LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlsCollectionTab();
	InitKeyBindingsCollectionTab(InOwningLocalPlayer);
	InitAccessibilityCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](const UListDataObject_Collection* AvailableTabCollection)-> bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);


	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());
	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;


	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.AddUnique(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}
	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.AddUnique(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	//This is the full code for constructor data interactor helper. See MACRO above
	/* TSharedRef<FOptionsDataInteractionHelper> ConstructedHelper =
		MakeShared<FOptionsDataInteractionHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty));*/

	//GameDifficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(GET_DESCRIPTION("GameDifficultyDescKey"));

		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Extreme"), FText::FromString(TEXT("Extreme")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplyChangeImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	//Test Item
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(UPharmadilloFunctionLibrary::GetOptionsSoftImageByTag(GD_GameplayTags::UI::Images::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT(""
			"The image to display can be specified in the project settings."
			"It can be anything the developer assigned in there")));


		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString("Overall Volume"));
			OverallVolume->SetDescriptionRichText(GET_DESCRIPTION("OverallVolumeDescKey"));

			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplyChangeImmediately(false);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString("Music Volume"));
			MusicVolume->SetDescriptionRichText(GET_DESCRIPTION("MusicVolumeDescKey"));

			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeImmediately(false);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		// SFX Volume 
		{
			UListDataObject_Scalar* SFXVolume = NewObject<UListDataObject_Scalar>();
			SFXVolume->SetDataID(FName("SFXVolume"));
			SFXVolume->SetDataDisplayName(FText::FromString("SFX Volume"));
			SFXVolume->SetDescriptionRichText(GET_DESCRIPTION("SFXVolumeDescKey"));

			SFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SFXVolume->SetSliderStepSize(0.01f);
			SFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			SFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSFXVolume));
			SFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSFXVolume));
			SFXVolume->SetShouldApplyChangeImmediately(false);

			VolumeCategoryCollection->AddChildListData(SFXVolume);
		}
	}

	//Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		//Allow Background Audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Background Audio.")));
			AllowBackgroundAudio->SetDescriptionRichText(FText::FromString(TEXT("NOT IMPLEMENTED YET.")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		//Use HDR Audio Mode
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode.")));
			UseHDRAudioMode->SetDescriptionRichText(FText::FromString(TEXT("NOT IMPLEMENTED YET.")));
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString("Enabled"));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString("Disabled"));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplyChangeImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;
	UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionsDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()-> bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in a package build</>"));


		//Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window")));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString("Fullscreen Mode"));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString("Borderless Window"));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString("Windowed Mode"));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);
			CreatedWindowMode = WindowMode;
			VideoTabCollection->AddChildListData(WindowMode);
		}

		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeImmediately(true);

			FOptionsDataEditConditionsDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()-> bool
				{
					if (CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen)
					{
						return false;
					}
					return true;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason
			(TEXT("\n\n<Disabled>Screen Resolution is not adjustable when the 'Window Mode' is set to Borderless Window. "
				"The value must match with the maximum allowed resolution</>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			ScreenResolution->AddEditCondition(WindowModeEditCondition);
			ScreenResolution->AddEditDependencyData(CreatedWindowMode);
			VideoTabCollection->AddChildListData(ScreenResolution);
		}
	}

	//Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();

		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetSoftDescriptionImage(UPharmadilloFunctionLibrary::GetOptionsSoftImageByTag(GD_GameplayTags::UI::Images::Frontend_Image_BrightnessSetter));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); //The default value Unreal has is 2.2f. so the range should encapsulate it
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));
			DisplayGamma->SetShouldApplyChangeImmediately(false);

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		//Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplyChangeImmediately(true);

			CreatedOverallQuality = OverallQuality;
			GraphicsCategoryCollection->AddChildListData(OverallQuality);
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("Model Resolution")));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplyChangeImmediately(false);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);
			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}


		//Global Illumination Quality
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplyChangeImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);
			//two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplyChangeImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ShadowQuality); //two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//AntiAliasing Quality
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("AntiAliasing")));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplyChangeImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality); //two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance")));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplyChangeImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality); //two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("TextureQuality")));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplyChangeImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(TextureQuality); //two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//Visual Effect Quality
		{
			UListDataObject_StringInteger* VisualEffectsQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectsQuality->SetDataID(FName("VisualEffectsQuality"));
			VisualEffectsQuality->SetDataDisplayName(FText::FromString(TEXT("VisualEffectsQuality")));
			VisualEffectsQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
			VisualEffectsQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VisualEffectsQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			VisualEffectsQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VisualEffectsQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VisualEffectsQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			VisualEffectsQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectsQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectsQuality->SetShouldApplyChangeImmediately(true);

			VisualEffectsQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(VisualEffectsQuality);
			//two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(VisualEffectsQuality);
		}
		
		//Foliage Quality
		{
			UListDataObject_StringInteger* FoliageQuality = NewObject<UListDataObject_StringInteger>();
			FoliageQuality->SetDataID(FName("FoliageQuality"));
			FoliageQuality->SetDataDisplayName(FText::FromString(TEXT("FoliageQuality")));
			FoliageQuality->SetDescriptionRichText(GET_DESCRIPTION("FoliageQualityDescKey"));
			FoliageQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			FoliageQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			FoliageQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			FoliageQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			FoliageQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			FoliageQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFoliageQuality));
			FoliageQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFoliageQuality));
			FoliageQuality->SetShouldApplyChangeImmediately(true);

			FoliageQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(FoliageQuality);
			//two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(FoliageQuality);
		}

		//Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("ReflectionQuality")));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplyChangeImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality); //two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		//Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("PostProcessingQuality")));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplyChangeImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);
			//two-way dependency when this is changed, the overall quality has to change also

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	// Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString("Advanced Graphics"));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplyChangeImmediately(true);

			FOptionsDataEditConditionsDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()-> bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>V-Sync can only be setted in fullscreen mode</>"));
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullScreenOnlyCondition);
			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//Frame Rate Limit
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString("Frame Rate Limit"));
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("Limitless")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplyChangeImmediately(true);


			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlsCollectionTab()
{
	UListDataObject_Collection* ControlsTabCollection = NewObject<UListDataObject_Collection>();
	ControlsTabCollection->SetDataID(FName("ControlsTabCollection"));
	ControlsTabCollection->SetDataDisplayName(FText::FromString("Controls"));

	//Haptics
	{
		UListDataObject_Collection* HapticsCategoryCollection = NewObject<UListDataObject_Collection>();
		HapticsCategoryCollection->SetDataID(FName("HapticsCategoryCollection"));
		HapticsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Haptics")));

		ControlsTabCollection->AddChildListData(HapticsCategoryCollection);

		UListDataObject_StringBool* CreatedAllowUIRumble = nullptr;
		UListDataObject_StringBool* CreatedAllowGamePlayRumble = nullptr;

		// UI Rumble
		{
			UListDataObject_StringBool* AllowUIRumble = NewObject<UListDataObject_StringBool>();
			AllowUIRumble->SetDataID(FName("AllowUIRumble"));
			AllowUIRumble->SetDataDisplayName(FText::FromString(TEXT("Controller Rumble - UI")));
			AllowUIRumble->SetDescriptionRichText(GET_DESCRIPTION("UIRumbleDescKey"));
			AllowUIRumble->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowUIRumble->OverrideFalseDisplayText(FText::FromString("Disabled"));
			AllowUIRumble->SetTrueAsDefaultValue();
			AllowUIRumble->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowUIControllerRumble));
			AllowUIRumble->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowUIControllerRumble));
			AllowUIRumble->SetShouldApplyChangeImmediately(true);

			CreatedAllowUIRumble = AllowUIRumble;
			HapticsCategoryCollection->AddChildListData(AllowUIRumble);
		}

		// UI Rumble
		{
			UListDataObject_StringBool* AllowGameplayRumble = NewObject<UListDataObject_StringBool>();
			AllowGameplayRumble->SetDataID(FName("AllowGameplayRumble"));
			AllowGameplayRumble->SetDataDisplayName(FText::FromString(TEXT("Controller Rumble - Game")));
			AllowGameplayRumble->SetDescriptionRichText(GET_DESCRIPTION("GameplayRumbleDescKey"));
			AllowGameplayRumble->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowGameplayRumble->OverrideFalseDisplayText(FText::FromString("Disabled"));
			AllowGameplayRumble->SetTrueAsDefaultValue();
			AllowGameplayRumble->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowGameplayControllerRumble));
			AllowGameplayRumble->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowGameplayControllerRumble));
			AllowGameplayRumble->SetShouldApplyChangeImmediately(true);

			CreatedAllowGamePlayRumble = AllowGameplayRumble;
			HapticsCategoryCollection->AddChildListData(AllowGameplayRumble);
		}

		//Controller Rumble Intensity
		{
			UListDataObject_Scalar* ControllerRumbleIntensity = NewObject<UListDataObject_Scalar>();
			ControllerRumbleIntensity->SetDataID(FName("ControllerRumbleIntensity"));
			ControllerRumbleIntensity->SetDataDisplayName(FText::FromString("Rumble Intensity"));
			ControllerRumbleIntensity->SetDescriptionRichText(GET_DESCRIPTION("RumbleIntensityDescKey"));

			ControllerRumbleIntensity->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ControllerRumbleIntensity->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ControllerRumbleIntensity->SetSliderStepSize(0.01f);
			ControllerRumbleIntensity->SetDefaultValueFromString(LexToString(1.f));
			ControllerRumbleIntensity->SetDisplayNumericType(ECommonNumericType::Percentage);
			ControllerRumbleIntensity->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			ControllerRumbleIntensity->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetHapticsIntensityMultiplier));
			ControllerRumbleIntensity->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetHapticsIntensityMultiplier));
			ControllerRumbleIntensity->SetShouldApplyChangeImmediately(false);

			FOptionsDataEditConditionsDescriptor RumbleEnabledCondition;
			RumbleEnabledCondition.SetEditConditionFunc(
				[CreatedAllowUIRumble, CreatedAllowGamePlayRumble]()-> bool
				{
					if (CreatedAllowUIRumble->GetCurrentValueAsBool() || CreatedAllowGamePlayRumble->GetCurrentValueAsBool())
					{
						return true;
					}
					return false;
				}
			);
			RumbleEnabledCondition.SetDisabledRichReason
				(TEXT("\n\n<Disabled>Rumble Intensity is not adjustable when UI and Gameplay- rumble is disabled</>"));

			ControllerRumbleIntensity->AddEditCondition(RumbleEnabledCondition);
			ControllerRumbleIntensity->AddEditDependencyData(CreatedAllowUIRumble);
			ControllerRumbleIntensity->AddEditDependencyData(CreatedAllowGamePlayRumble);
			HapticsCategoryCollection->AddChildListData(ControllerRumbleIntensity);
		}
	}
	
	//LookSettings
	{
		UListDataObject_Collection* LookCategoryCollection = NewObject<UListDataObject_Collection>();
		LookCategoryCollection->SetDataID(FName("LookSettings"));
		LookCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Look Settings")));

		ControlsTabCollection->AddChildListData(LookCategoryCollection);
		
		// Invert
		{
			UListDataObject_StringBool* InvertLook = NewObject<UListDataObject_StringBool>();
			InvertLook->SetDataID(FName("InvertLook"));
			InvertLook->SetDataDisplayName(FText::FromString(TEXT("Look Direction")));
			InvertLook->SetDescriptionRichText(GET_DESCRIPTION("InvertLookDescKey"));
			InvertLook->OverrideTrueDisplayText(FText::FromString("Inverted"));
			InvertLook->OverrideFalseDisplayText(FText::FromString("Normal"));
			InvertLook->SetFalseAsDefaultValue();
			InvertLook->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetInvertLook));
			InvertLook->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetInvertLook));
			InvertLook->SetShouldApplyChangeImmediately(true);
			
			LookCategoryCollection->AddChildListData(InvertLook);
		}
		
		//Look Sensitivity
		{
			UListDataObject_Scalar* LookSensitivity = NewObject<UListDataObject_Scalar>();
			LookSensitivity->SetDataID(FName("LookSensitivity"));
			LookSensitivity->SetDataDisplayName(FText::FromString("Look Sensitivity"));
			LookSensitivity->SetDescriptionRichText(GET_DESCRIPTION("LookSensitivityDescKey"));

			LookSensitivity->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			LookSensitivity->SetOutputValueRange(TRange<float>(0.1f, 1.9f));
			LookSensitivity->SetSliderStepSize(0.01f);
			LookSensitivity->SetDefaultValueFromString(LexToString(1.f));
			LookSensitivity->SetDisplayNumericType(ECommonNumericType::Percentage);
			LookSensitivity->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			LookSensitivity->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetLookSensitivity));
			LookSensitivity->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetLookSensitivity));
			LookSensitivity->SetShouldApplyChangeImmediately(false);
			
			LookCategoryCollection->AddChildListData(LookSensitivity);
		}
		
		//Camera Follow Roll Movement
		{
			UListDataObject_StringBool* AllowCameraFollowRollMovement = NewObject<UListDataObject_StringBool>();
			AllowCameraFollowRollMovement->SetDataID(FName("AllowCameraFollowRollMovement"));
			AllowCameraFollowRollMovement->SetDataDisplayName(FText::FromString(TEXT("Allow Camera Follow Roll Movement.")));
			AllowCameraFollowRollMovement->SetDescriptionRichText(GET_DESCRIPTION("AllowCameraFollowRollMovementDescKey"));
			AllowCameraFollowRollMovement->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowCameraFollowRollMovement->OverrideFalseDisplayText(FText::FromString("Disabled"));
			AllowCameraFollowRollMovement->SetTrueAsDefaultValue();
			AllowCameraFollowRollMovement->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowCameraFollowRollMovement));
			AllowCameraFollowRollMovement->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowCameraFollowMovement));
			AllowCameraFollowRollMovement->SetShouldApplyChangeImmediately(true);

			LookCategoryCollection->AddChildListData(AllowCameraFollowRollMovement);
		}
	}
	
	//Movement settings
	{
		UListDataObject_Collection* MovementCategoryCollection = NewObject<UListDataObject_Collection>();
		MovementCategoryCollection->SetDataID(FName("MovementSettings"));
		MovementCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Movement Settings")));

		ControlsTabCollection->AddChildListData(MovementCategoryCollection);
		
		//Sprint Handling
		{
			UListDataObject_StringBool* AllowToggleSprint = NewObject<UListDataObject_StringBool>();
			AllowToggleSprint->SetDataID(FName("AllowToggleSprint"));
			AllowToggleSprint->SetDataDisplayName(FText::FromString(TEXT("Sprint mode")));
			AllowToggleSprint->SetDescriptionRichText(GET_DESCRIPTION("AllowToggleSprintDescKey"));
			AllowToggleSprint->OverrideTrueDisplayText(FText::FromString("Toggle"));
			AllowToggleSprint->OverrideFalseDisplayText(FText::FromString("Hold"));
			AllowToggleSprint->SetTrueAsDefaultValue();
			AllowToggleSprint->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowToggleSprint));
			AllowToggleSprint->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowToggleSprint));
			AllowToggleSprint->SetShouldApplyChangeImmediately(true);

			MovementCategoryCollection->AddChildListData(AllowToggleSprint);
		}
	}

	RegisteredOptionsTabCollections.Add(ControlsTabCollection);
}

void UOptionsDataRegistry::InitKeyBindingsCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* KeyBindingTabCollection = NewObject<UListDataObject_Collection>();
	KeyBindingTabCollection->SetDataID(FName("KeyBindingTabCollection"));
	KeyBindingTabCollection->SetDataDisplayName(FText::FromString("Key Bindings"));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);
	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();
	check(EIUserSettings);

	// KeyBinding Categories
	{
		//Movement
		UListDataObject_Collection* MovementCategoryCollection = NewObject<UListDataObject_Collection>();
		MovementCategoryCollection->SetDataID(FName("Movement"));
		MovementCategoryCollection->SetDataDisplayName(FText::FromString("Movement"));

		KeyBindingTabCollection->AddChildListData(MovementCategoryCollection);

		//Combat
		UListDataObject_Collection* CombatCategoryCollection = NewObject<UListDataObject_Collection>();
		CombatCategoryCollection->SetDataID(FName("Combat"));
		CombatCategoryCollection->SetDataDisplayName(FText::FromString("Combat"));

		KeyBindingTabCollection->AddChildListData(CombatCategoryCollection);

		// Misc
		UListDataObject_Collection* OtherCategoryCollection = NewObject<UListDataObject_Collection>();
		OtherCategoryCollection->SetDataID(FName("Misc"));
		OtherCategoryCollection->SetDataDisplayName(FText::FromString("Miscellaneous"));

		KeyBindingTabCollection->AddChildListData(OtherCategoryCollection);
	}

	//Inputs
	{
		for (const TPair<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
		{
			TObjectPtr<UEnhancedPlayerMappableKeyProfile> MappableKeyProfile = ProfilePair.Value;
			check(MappableKeyProfile);

			for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
			{
				UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
				KeyRemapDataObject->SetDataID(MappingRowPair.Key);

				FName DisplayCategory;
				FPlayerKeyMapping KeyboardMapping;
				FPlayerKeyMapping GamepadMapping;
				for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
				{
					KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
					if (KeyMapping.GetSlot() == EPlayerMappableKeySlot::First) //Keyboard bindings are always the first
					{
						KeyboardMapping = KeyMapping;
					}
					else if (KeyMapping.GetSlot() == EPlayerMappableKeySlot::Second) //Gamepad bindings are always the first
					{
						GamepadMapping = KeyMapping;
					}
					DisplayCategory = FName(*KeyMapping.GetDisplayCategory().ToString());
				}
				KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, KeyboardMapping, GamepadMapping);

				KeyBindingTabCollection->FindChildListDataFromID(DisplayCategory)->AddChildListData(KeyRemapDataObject);
			}
		}
	}


	RegisteredOptionsTabCollections.Add(KeyBindingTabCollection);
}

void UOptionsDataRegistry::InitAccessibilityCollectionTab()
{
	UListDataObject_Collection* AccessibilityTabCollection = NewObject<UListDataObject_Collection>();
	AccessibilityTabCollection->SetDataID(FName("AccessibilityTabCollection"));
	AccessibilityTabCollection->SetDataDisplayName(FText::FromString("Accessibility"));

	RegisteredOptionsTabCollections.Add(AccessibilityTabCollection);
}
