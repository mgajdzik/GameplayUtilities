// Copyright Mateusz Gajdzik. All Rights Reserved.

#include "AnimNodes/AnimNode_BlendListByTag.h"

void FAnimNode_BlendListByTag::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	FAnimNode_BlendListBase::Initialize_AnyThread(Context);
	for (int i = 0; i < GameplayTags.Num(); i++)
	{
		TagToPoseIndex.Add(GameplayTags[i], i + 1);
	}
}

int32 FAnimNode_BlendListByTag::GetActiveChildIndex()
{
	int32* PoseIndex = TagToPoseIndex.Find(ActiveTagValue);
	if (PoseIndex)
	{
		return *PoseIndex;
	}
	return 0;
}

void FAnimNode_BlendListByTag::AddPose()
{
	Super::AddPose();
	if (BlendPose.Num() > 1)
	{
		GameplayTags.AddDefaulted();
	}
}

void FAnimNode_BlendListByTag::RemovePose(int32 PoseIndex)
{
	ensure(PoseIndex > 0);
	Super::RemovePose(PoseIndex);
	GameplayTags.RemoveAt(PoseIndex - 1);
}
