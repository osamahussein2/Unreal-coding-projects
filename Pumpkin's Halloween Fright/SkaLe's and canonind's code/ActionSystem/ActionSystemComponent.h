// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionSystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStart, FGameplayTag, ActionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStop, FGameplayTag, ActionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveTagsUpdated, FGameplayTagContainer, ActiveGameplayTags);

class UASAction;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionSystemComponent();

	UFUNCTION()
	void HandleAnimNotifyActionEvent(FName EventName);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void GrantActiveTags(const FGameplayTagContainer& InTags);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void ClearActiveTags(const FGameplayTagContainer& InTags);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void GiveAction(UASAction* InAction);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void ClearAction(UASAction* InAction);

	UFUNCTION(Category = "Action System")
	void AddActiveAction(UASAction* InAction);

	UFUNCTION(Category = "Action System")
	void ClearActiveAction(UASAction* InAction);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StartActionByTag(const FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StopActionByTag(const FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool IsActionRunning(const FGameplayTag ActionTag) const;

	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool CanActionRestartSelf(const FGameplayTag ActionTag) const;

	float PlayActionMontage(class UAnimMontage* MontageToPlay, const float PlayRate = 1.f, const FName& StartSection = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void SetAvatarActor(AActor* InActor) {AvatarActor = InActor;}

	UFUNCTION(BlueprintCallable, Category = "Action System")
	AActor* GetAvatarActor() const;

protected:

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActionStart OnActionStart;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActionStop OnActionStop;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActiveTagsUpdated OnActiveTagsUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
protected:
	UPROPERTY()
	TArray<TObjectPtr<UASAction>> AvailableActions;

	UPROPERTY()
	TArray<TObjectPtr<UASAction>> ActiveActions;

	UPROPERTY(EditAnywhere, Category = "Action System")
	TArray<TObjectPtr<UASAction>> DefaultActions;

protected:
	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;

};
