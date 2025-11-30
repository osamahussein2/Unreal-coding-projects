// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainMenuCharacter.generated.h"

//class UMainMenuWidget_MobileVersion;

UCLASS()
class SHROOMJAM2025_API AMainMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainMenuCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EnableUIInput();

	//void InitializeMainMenuMobileWidget();

private:
	// Mobile properties and variables
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget_MobileVersion> mainMenuMobileWidgetClass;*/
};
