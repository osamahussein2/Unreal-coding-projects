// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ASAction.generated.h"

class UActionSystemComponent;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FActionMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Action")
	UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	float PlayRate;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FName> MontageSectionNames;

	// May be add random montage bool

	FActionMontage()
		: ActionMontage(nullptr)
		, PlayRate(1.0f)
		, MontageSectionNames({})
	{}

	bool IsValid() const { return ActionMontage != nullptr; }
	int32 GetLastSectionIndex() const { return MontageSectionNames.Num() - 1; }

	FName GetMontageSectionName(const int32 SectionIndex) const
	{
		if (MontageSectionNames.IsEmpty()) return NAME_None;
		if (!MontageSectionNames.IsValidIndex(SectionIndex)) return NAME_None;
		return MontageSectionNames[SectionIndex];
	}
};

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual bool OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName) { return false; }

	virtual void StartAction(UActionSystemComponent* ActionComp);
	virtual void StopAction(UActionSystemComponent* ActionComp);
	virtual void RestartAction(UActionSystemComponent* ActionComp);
	virtual bool CanStartAction(UActionSystemComponent* ActionComp) const;
	virtual bool CanStopAction(UActionSystemComponent* ActionComp) const;
	bool DoesSatisfyTagRequirements(const FGameplayTagContainer& ActiveGameplayTags) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	FGameplayTag ActionTag;

	UPROPERTY(EditAnywhere, Category = "Action")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditAnywhere, Category = "Action")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	FActionMontage ActionMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	bool bCanSelfRestart = false;
};
