// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicTickingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComponentTicked, float, DeltaTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYUTILITIES_API UDynamicTickingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UDynamicTickingComponent(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		PrimaryComponentTick.bCanEverTick = true;
	}

	UDynamicTickingComponent() : Super()
	{
		PrimaryComponentTick.bCanEverTick = true;
	}

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	

	UFUNCTION(BlueprintCallable)
	static UDynamicTickingComponent* GetDynamicComponentFromActor(AActor* Actor, TSubclassOf<UDynamicTickingComponent> ComponentClass);

	UPROPERTY(BlueprintAssignable)
	FOnComponentTicked OnComponentTicked;
};


