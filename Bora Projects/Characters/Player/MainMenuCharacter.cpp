// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/MainMenuCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainMenuCharacter::AMainMenuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMainMenuCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(this, TEXT("/Game/Music/main_menu_1")));
	
	// Only for developing the build on mobile devices
	if (mainMenuTouchInterface && Cast<APlayerController>(GetController()))
	{
		Cast<APlayerController>(GetController())->ActivateTouchInterface(mainMenuTouchInterface);
		isTouchInterfaceEnabled = true;
	}
}

// Called every frame
void AMainMenuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

