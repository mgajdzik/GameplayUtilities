// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DynamicTickingComponent.h"


void UDynamicTickingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnComponentTicked.Broadcast(DeltaTime);
}

UDynamicTickingComponent* UDynamicTickingComponent::GetDynamicComponentFromActor(AActor* Actor,
	TSubclassOf<UDynamicTickingComponent> ComponentClass)
{
	if (!IsValid(Actor)) return nullptr;
	TArray<UDynamicTickingComponent*> Components;
	Actor->GetComponents(Components);
	UDynamicTickingComponent* Result = nullptr;
	for (UDynamicTickingComponent* Component : Components)
	{
		if (Component->GetClass() == ComponentClass)
		{
			Result = Component;
			break;
		}		
	}
	
	if (!IsValid(Result))
	{
		Result = NewObject<UDynamicTickingComponent>(Actor, ComponentClass);
		if (!IsValid(Result)) return nullptr;
		Result->RegisterComponent();
	}
	return Result;
}

