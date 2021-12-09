// Copyright Mateusz Gajdzik. All Rights Reserved.

#include "AnimNodes/AnimGraphNode_BlendListByTag.h"
#include "Textures/SlateIcon.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "ScopedTransaction.h"
#include "Kismet2/CompilerResultsLog.h"
#include "BlueprintNodeSpawner.h"


#define LOCTEXT_NAMESPACE "BlendListByTag"

/////////////////////////////////////////////////////

UAnimGraphNode_BlendListByTag::UAnimGraphNode_BlendListByTag(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Make sure we start out with a pin
	Node.AddPose();
}

FString UAnimGraphNode_BlendListByTag::GetNodeCategory() const
{
	return "Blend Poses by gameplay tag";
}

FText UAnimGraphNode_BlendListByTag::GetTooltipText() const
{
	return LOCTEXT("UAnimGraphNode_BlendListByTag_Title", "Blend Poses (by gameplay tag)");
}

FText UAnimGraphNode_BlendListByTag::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("UAnimGraphNode_BlendListByTag_Title", "Blend Poses (by gameplay tag)");
}

void UAnimGraphNode_BlendListByTag::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (!Context->bIsDebugging)
	{
		FToolMenuSection* Section = &Menu->AddSection("AnimGraphNodeAddElementPin", LOCTEXT("ExposeHeader", "Gameplay tag poses"));
		if (Context->Pin && (Context->Pin->Direction == EGPD_Input))
		{
			int32 RawArrayIndex = 0;
			bool bIsPosePin = false;
			bool bIsTimePin = false;
			GetPinInformation(Context->Pin->PinName.ToString(), /*out*/ RawArrayIndex, /*out*/ bIsPosePin, /*out*/ bIsTimePin);

			if (bIsPosePin || bIsTimePin)
			{
				const int32 ExposedIndex = RawArrayIndex - 1;

				if (ExposedIndex != INDEX_NONE)
				{
					// Offer to remove this specific pin
					const FUIAction Action = FUIAction( FExecuteAction::CreateUObject( const_cast<UAnimGraphNode_BlendListByTag*>(this), &UAnimGraphNode_BlendListByTag::RemovePinFromBlendList, const_cast<UEdGraphPin*>(Context->Pin)) );
					Section->AddMenuEntry("RemovePose", LOCTEXT("RemovePose", "Remove Pose"), FText::GetEmpty(), FSlateIcon(), Action);
				}
			}
		}
		

		const FUIAction Action = FUIAction( FExecuteAction::CreateUObject( const_cast<UAnimGraphNode_BlendListByTag*>(this), &UAnimGraphNode_BlendListByTag::ExposeTagElementAsPin) );
		Section->AddMenuEntry(NAME_None, LOCTEXT("AddPose", "Add new Pose"), LOCTEXT("AddPose", "Add new Pose"), FSlateIcon(), Action);
		
	}
}

void UAnimGraphNode_BlendListByTag::ExposeTagElementAsPin()
{
	Node.AddPose();
	ReconstructNode();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UAnimGraphNode_BlendListByTag::RemovePinFromBlendList(UEdGraphPin* Pin)
{
	int32 RawArrayIndex = 0;
	bool bIsPosePin = false;
	bool bIsTimePin = false;
	GetPinInformation(Pin->PinName.ToString(), /*out*/ RawArrayIndex, /*out*/ bIsPosePin, /*out*/ bIsTimePin);

	if (bIsPosePin || bIsTimePin)
	{
		FScopedTransaction Transaction( LOCTEXT("RemovePin", "RemovePin") );
		Modify();

		// Remove the pose from the node
		FProperty* AssociatedProperty;
		int32 ArrayIndex;
		GetPinAssociatedProperty(GetFNodeType(), Pin, /*out*/ AssociatedProperty, /*out*/ ArrayIndex);

		ensure(ArrayIndex == RawArrayIndex);

		// setting up removed pins info 
		RemovedPinArrayIndex = ArrayIndex;
		Node.RemovePose(ArrayIndex);
		Pin->SetSavePinIfOrphaned(false);
		ReconstructNode();
		//@TODO: Just want to invalidate the visual representation currently
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

void UAnimGraphNode_BlendListByTag::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	if ((PropertyName == TEXT("GameplayTags")))
	{
		ReconstructNode();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UAnimGraphNode_BlendListByTag::GetPinInformation(const FString& InPinName, int32& Out_PinIndex, bool& Out_bIsPosePin, bool& Out_bIsTimePin)
{
	const int32 UnderscoreIndex = InPinName.Find(TEXT("_"), ESearchCase::CaseSensitive);
	if (UnderscoreIndex != INDEX_NONE)
	{
		const FString ArrayName = InPinName.Left(UnderscoreIndex);
		Out_PinIndex = FCString::Atoi(*(InPinName.Mid(UnderscoreIndex + 1)));

		Out_bIsPosePin = (ArrayName == TEXT("BlendPose"));
		Out_bIsTimePin = (ArrayName == TEXT("BlendTime"));
	}
	else
	{
		Out_bIsPosePin = false;
		Out_bIsTimePin = false;
		Out_PinIndex = INDEX_NONE;
	}
}

void UAnimGraphNode_BlendListByTag::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
	bool bIsPosePin;
	bool bIsTimePin;
	int32 RawArrayIndex;
	GetPinInformation(Pin->PinName.ToString(), /*out*/ RawArrayIndex, /*out*/ bIsPosePin, /*out*/ bIsTimePin);
	checkSlow(RawArrayIndex == ArrayIndex);

	if (!(bIsPosePin || bIsTimePin))
	{
		return;
	}
	if (RawArrayIndex > 0)
	{
		const int32 ExposedPinIndex = RawArrayIndex - 1;

		// find pose index and see if it's mapped already or not
		if (Node.GameplayTags.Num() > ExposedPinIndex && Node.GameplayTags[ExposedPinIndex] != FGameplayTag::EmptyTag)
		{
			Pin->PinFriendlyName = FText::FromString(Node.GameplayTags[ExposedPinIndex].ToString());
		}
		else
		{
			Pin->PinFriendlyName = LOCTEXT("EmptyTag", "Empty Tag");
		}
	}
	else if (ensure(RawArrayIndex == 0))
	{
		Pin->PinFriendlyName = LOCTEXT("Default", "Default");
	}

	if (bIsTimePin)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("PinFriendlyName"), Pin->PinFriendlyName);
		Pin->PinFriendlyName = FText::Format(LOCTEXT("FriendlyNameBlendTime", "{PinFriendlyName} Blend Time"), Args);
	}
		
	
}

#undef LOCTEXT_NAMESPACE
