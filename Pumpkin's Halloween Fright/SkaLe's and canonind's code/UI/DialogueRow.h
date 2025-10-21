#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FDialogueRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DialogueID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LineIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplaySpeed = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayAfter = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Portrait;
};
