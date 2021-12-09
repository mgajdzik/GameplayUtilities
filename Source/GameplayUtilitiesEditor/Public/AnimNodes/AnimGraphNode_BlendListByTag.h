// Copyright Mateusz Gajdzik. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameplayUtilities/Public/AnimNodes/AnimNode_BlendListByTag.h"

#include "AnimGraph/Classes/AnimGraphNode_BlendListBase.h"
#include "AnimGraphNode_BlendListByTag.generated.h"
class FBlueprintActionDatabaseRegistrar;

UCLASS(MinimalAPI)
class UAnimGraphNode_BlendListByTag : public UAnimGraphNode_BlendListBase
{
	GENERATED_BODY()

public:
	UAnimGraphNode_BlendListByTag(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Category=Settings)
	FAnimNode_BlendListByTag Node;
	
	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	// End of UK2Node interface

	// UAnimGraphNode_Base interface
	virtual FString GetNodeCategory() const override;

	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const override;
	// End of UAnimGraphNode_Base interface

	//@TODO: Generalize this behavior (returning a list of actions/delegates maybe?)
	virtual void RemovePinFromBlendList(UEdGraphPin* Pin);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
protected:
	void ExposeTagElementAsPin();

	// Gets information about the specified pin.  If both bIsPosePin and bIsTimePin are false, the index is meaningless
	static void GetPinInformation(const FString& InPinName, int32& Out_PinIndex, bool& Out_bIsPosePin, bool& Out_bIsTimePin);

};