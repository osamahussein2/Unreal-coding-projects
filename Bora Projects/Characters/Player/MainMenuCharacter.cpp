// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/MainMenuCharacter.h"
#include "Kismet/GameplayStatics.h"
//#include "Widgets/MainMenuWidget_MobileVersion.h"

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

void AMainMenuCharacter::EnableUIInput()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	if (Cast<APlayerController>(GetController()))
	{
		Cast<APlayerController>(GetController())->SetInputMode(GameInputMode);
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Don't worry about mouse lock

	if (Cast<APlayerController>(GetController()))
	{
		Cast<APlayerController>(GetController())->SetInputMode(InputMode);

		if (Cast<APlayerController>(GetController())->bShowMouseCursor != true)
		{
			Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		}
	}
}

//void AMainMenuCharacter::InitializeMainMenuMobileWidget()
//{
//	// Only for developing the build on mobile devices
//	if (mainMenuMobileWidgetClass)
//	{
//		UMainMenuWidget_MobileVersion* mainMenuMobileWidget = CreateWidget<UMainMenuWidget_MobileVersion>
//			(Cast<APlayerController>(GetController()), mainMenuMobileWidgetClass);
//
//		if (mainMenuMobileWidget)
//		{
//			mainMenuMobileWidget->AddToViewport();
//			mainMenuMobileWidget = nullptr;
//		}
//	}
//}