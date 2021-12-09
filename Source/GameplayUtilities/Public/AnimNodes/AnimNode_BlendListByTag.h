// Copyright Mateusz Gajdzik. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AnimGraphRuntime/Public/AnimNodes/AnimNode_BlendListBase.h"
#include "AnimNode_BlendListByTag.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct GAMEPLAYUTILITIES_API FAnimNode_BlendListByTag : public FAnimNode_BlendListBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category=Links)
	TArray<FGameplayTag> GameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Runtime, meta=(AlwaysAsPin))
	mutable FGameplayTag ActiveTagValue;

	FAnimNode_BlendListByTag() : FAnimNode_BlendListBase()
	{
	}

	virtual void AddPose() override;
	virtual void RemovePose(int32 PoseIndex) override;
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	
protected:
	TMap<FGameplayTag, int32> TagToPoseIndex;

	virtual int32 GetActiveChildIndex() override;
	virtual FString GetNodeName(FNodeDebugData& DebugData) override { return DebugData.GetNodeName(this); }
	

};
