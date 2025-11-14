// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Delegates/Delegate.h"
#include "GameplayTagComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagChanged, FName, AddedTag, FName, RemoveTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagAdded, FName, AddedTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagRemoved, FName, RemoveTag);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHROOMJAM2025_API UGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayTagComponent();

	void RemoveActionTag(FName tagName_);
	void AddActionTag(FName tagName_);

	bool HasActionTag(FName tagName_);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayTagChanged GameplayTagChanged;

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayTagAdded GameplayTagAdded;

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayTagRemoved GameplayTagRemoved;

private:
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer GameplayTags;
};
