// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddAction, FString, action_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveAction, FString, action_);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSGAMEJAM_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	FOnAddAction OnAddAction;
	FOnRemoveAction OnRemoveAction;

	bool HasAction(FString action_) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Action functions
	UFUNCTION()
	void AddAction(FString action_);

	UFUNCTION()
	void RemoveAction(FString action_);

private:
	// Player actions array
	UPROPERTY(VisibleAnywhere, Category = "Actions")
	TArray<FString> actions;
};
