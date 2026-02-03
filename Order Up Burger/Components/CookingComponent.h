// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlayerCharacter.h"
#include "CineCameraActor.h"
#include "CookingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIOBUILDERS_API UCookingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCookingComponent();

	void ToggleCookingState(EPlayerState& playerState_, bool& overlayVisibility_, 
		FVector& cameraLocation_, FRotator& cameraRotation_, AActor* actor_, FVector& characterLocation_, 
		APlayerController* playerController_, FName socketName_);

	void DestroyReferences();

	UFUNCTION()
	ACineCameraActor* GetCineCamera() const { return cineCamera; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void AttachFoodOnOwner(USkeletalMeshComponent* skeletalMeshComp_, float foodAttachOffsetZ_, AActor* attachingActor_, 
		FName& socketName_);

	UPROPERTY()
	ACineCameraActor* cineCamera = nullptr;
};
