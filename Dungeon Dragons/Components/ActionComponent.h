// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayActiveTagAdded, TArray<FGameplayTag>, AddedTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayActiveTagRemoved, TArray<FGameplayTag>, RemovedTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayBlockedTagAdded, TArray<FGameplayTag>, AddedTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayBlockedTagRemoved, TArray<FGameplayTag>, RemovedTags);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRAWLER2026_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayActiveTagAdded GameplayActiveTagsAdded;

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayActiveTagRemoved GameplayActiveTagsRemoved;

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayBlockedTagAdded GameplayBlockedTagsAdded;

	UPROPERTY(VisibleAnywhere, Category = "Events")
	FOnGameplayBlockedTagRemoved GameplayBlockedTagsRemoved;

	bool HasBlockedTag(FGameplayTag BlockedTag);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void RemoveActiveActionTags(TArray<FGameplayTag> RemovedTags);
	UFUNCTION()
	void RemoveBlockedActionTags(TArray<FGameplayTag> RemovedTags);

	UFUNCTION()
	void AddActiveActionTags(TArray<FGameplayTag> AddedTags);
	UFUNCTION()
	void AddBlockedActionTags(TArray<FGameplayTag> AddedTags);

private:
	UPROPERTY(VisibleAnywhere, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(VisibleAnywhere, Category = "Tags")
	FGameplayTagContainer BlockedGameplayTags;
};
