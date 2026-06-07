#pragma once

#include "CoreMinimal.h"
#include "CableConnection.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct FCableConnection
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ConnectedActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Socket = nullptr;
};