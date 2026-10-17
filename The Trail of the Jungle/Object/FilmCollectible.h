// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure/ThreeLaneSignInfo.h"
#include "FilmCollectible.generated.h"

class UBoxComponent;
class APlayerCharacter;
class ASign;
class AFilmPickupMetasound;

UCLASS()
class COMFYAUTUMNJAM_API AFilmCollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFilmCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void FilmCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void FilmLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void PlayFilmPickedUpMetasound();

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> boxComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> staticMeshComponent = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> overlappedPlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sign Reference")
	TObjectPtr<ASign> referencedSign = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sign Reference")
	FThreeLaneSignInfo threeLaneSignInfo = FThreeLaneSignInfo();

	UPROPERTY(EditDefaultsOnly, Category = "Film Pickup Metasound Settings")
	TSubclassOf<AFilmPickupMetasound> filePickupMetaSoundClass = nullptr;
};
