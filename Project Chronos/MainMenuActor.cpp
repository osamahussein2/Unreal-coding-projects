// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuActor.h"
#include "PlayerCharacter.h"

// Sets default values
AMainMenuActor::AMainMenuActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize any variables
	mainMenuUI = nullptr;
}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();

	// Create the main menu widget and add it to viewport
	mainMenuUI = CreateWidget<UMainMenuWidget>(GetWorld()->GetFirstPlayerController(), mainMenuClass);
	mainMenuUI->AddToViewport();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

	// Make sure the mouse cursor is turned on for clicking on main menu buttons
	if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != true)
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	if (APlayerCharacter::isInMenu != true) APlayerCharacter::isInMenu = true;

	// Play the main menu music
	if (mainMenuMusic) UGameplayStatics::PlaySound2D(this, mainMenuMusic);
}

void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mainMenuUI)
	{
		mainMenuUI->SetMoveForwardControlsText();
		mainMenuUI->SetMoveBackControlsText();
		mainMenuUI->SetMoveLeftControlsText();
		mainMenuUI->SetMoveRightControlsText();

		mainMenuUI->SetPauseControlsText();
		mainMenuUI->SetAttackControlsText();

		// Call the change keys functions
		ChangeForwardKey();
		ChangeBackKey();
		ChangeLeftKey();
		ChangeRightKey();
		
		ChangePauseKey();
		ChangeAttackKey();
	}
}

void AMainMenuActor::ChangeForwardKey()
{
	if (APlayerCharacter::modifyMoveForwardKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::W;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::S;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::A;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::D;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Q;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::E;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::R;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::T;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Y;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::U;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::I;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::O;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::P;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::G;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::H;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::J;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::K;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::L;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Z;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::X;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::C;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::V;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::B;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::N;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::M;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::CapsLock;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::LeftShift;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::LeftControl;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::LeftAlt;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::SpaceBar;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Backslash;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::RightControl;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::RightShift;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Enter;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::BackSpace;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Equals;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Hyphen;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Zero;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::One;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Two;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Three;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Four;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Five;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Six;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Seven;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Eight;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Nine;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Tilde;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Home;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::PageUp;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::PageDown;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::End;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::LeftBracket;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::RightBracket;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Slash;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Semicolon;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Colon;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Quote;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Comma;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Period;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::RightParantheses;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Escape;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Underscore;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F1;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F2;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F3;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F4;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F5;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F6;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F7;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F8;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F9;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F10;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F11;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::F12;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Left;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Right;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Up;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Down;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Tab;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::Delete;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentMoveForwardKey = EKeys::RightAlt;
			APlayerCharacter::modifyMoveForwardKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) || 
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyMoveForwardKey = false;
		}
	}
}

void AMainMenuActor::ChangeBackKey()
{
	if (APlayerCharacter::modifyMoveBackKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::W;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::S;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::A;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::D;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Q;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::E;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::R;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::T;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Y;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::U;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::I;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::O;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::P;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::G;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::H;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::J;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::K;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::L;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Z;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::X;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::C;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::V;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::B;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::N;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::M;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::CapsLock;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::LeftShift;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::LeftControl;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::LeftAlt;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::SpaceBar;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Backslash;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::RightControl;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::RightShift;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Enter;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::BackSpace;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Equals;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Hyphen;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Zero;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::One;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Two;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Three;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Four;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Five;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Six;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Seven;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Eight;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Nine;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Tilde;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Home;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::PageUp;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::PageDown;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::End;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::LeftBracket;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::RightBracket;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Slash;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Semicolon;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Colon;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Quote;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Comma;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Period;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::RightParantheses;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Escape;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Underscore;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F1;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F2;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F3;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F4;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F5;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F6;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F7;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F8;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F9;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F10;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F11;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::F12;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Left;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Right;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Up;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Down;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Tab;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::Delete;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentMoveBackKey = EKeys::RightAlt;
			APlayerCharacter::modifyMoveBackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyMoveBackKey = false;
		}
	}
}

void AMainMenuActor::ChangeLeftKey()
{
	if (APlayerCharacter::modifyMoveLeftKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::W;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::S;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::A;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::D;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Q;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::E;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::R;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::T;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Y;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::U;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::I;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::O;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::P;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::G;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::H;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::J;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::K;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::L;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Z;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::X;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::C;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::V;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::B;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::N;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::M;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::CapsLock;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::LeftShift;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::LeftControl;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::LeftAlt;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::SpaceBar;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Backslash;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::RightControl;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::RightShift;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Enter;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::BackSpace;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Equals;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Hyphen;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Zero;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::One;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Two;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Three;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Four;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Five;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Six;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Seven;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Eight;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Nine;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Tilde;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Home;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::PageUp;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::PageDown;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::End;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::LeftBracket;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::RightBracket;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Slash;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Semicolon;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Colon;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Quote;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Comma;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Period;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::RightParantheses;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Escape;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Underscore;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F1;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F2;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F3;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F4;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F5;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F6;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F7;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F8;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F9;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F10;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F11;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::F12;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Left;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Right;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Up;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Down;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Tab;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::Delete;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentMoveLeftKey = EKeys::RightAlt;
			APlayerCharacter::modifyMoveLeftKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyMoveLeftKey = false;
		}
	}
}

void AMainMenuActor::ChangeRightKey()
{
	if (APlayerCharacter::modifyMoveRightKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::W;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::S;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::A;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::D;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Q;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::E;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::R;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::T;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Y;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::U;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::I;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::O;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::P;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::G;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::H;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::J;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::K;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::L;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Z;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::X;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::C;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::V;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::B;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::N;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::M;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::CapsLock;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::LeftShift;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::LeftControl;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::LeftAlt;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::SpaceBar;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Backslash;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::RightControl;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::RightShift;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Enter;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::BackSpace;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Equals;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Hyphen;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Zero;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::One;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Two;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Three;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Four;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Five;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Six;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Seven;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Eight;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Nine;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Tilde;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Home;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::PageUp;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::PageDown;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::End;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::LeftBracket;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::RightBracket;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Slash;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Semicolon;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Colon;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Quote;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Comma;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Period;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::RightParantheses;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Escape;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Underscore;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F1;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F2;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F3;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F4;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F5;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F6;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F7;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F8;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F9;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F10;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F11;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::F12;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Left;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Right;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Up;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Down;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Tab;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::Delete;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentMoveRightKey = EKeys::RightAlt;
			APlayerCharacter::modifyMoveRightKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyMoveRightKey = false;
		}
	}
}

void AMainMenuActor::ChangePauseKey()
{
	if (APlayerCharacter::modifyPauseKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentPauseKey = EKeys::W;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentPauseKey = EKeys::S;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentPauseKey = EKeys::A;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentPauseKey = EKeys::D;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentPauseKey = EKeys::Q;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentPauseKey = EKeys::E;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentPauseKey = EKeys::R;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentPauseKey = EKeys::T;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentPauseKey = EKeys::Y;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentPauseKey = EKeys::U;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentPauseKey = EKeys::I;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentPauseKey = EKeys::O;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentPauseKey = EKeys::P;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentPauseKey = EKeys::F;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentPauseKey = EKeys::G;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentPauseKey = EKeys::H;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentPauseKey = EKeys::J;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentPauseKey = EKeys::K;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentPauseKey = EKeys::L;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentPauseKey = EKeys::Z;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentPauseKey = EKeys::X;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentPauseKey = EKeys::C;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentPauseKey = EKeys::V;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentPauseKey = EKeys::B;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentPauseKey = EKeys::N;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentPauseKey = EKeys::M;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentPauseKey = EKeys::CapsLock;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentPauseKey = EKeys::LeftShift;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentPauseKey = EKeys::LeftControl;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentPauseKey = EKeys::LeftAlt;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentPauseKey = EKeys::SpaceBar;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentPauseKey = EKeys::Backslash;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentPauseKey = EKeys::RightControl;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentPauseKey = EKeys::RightShift;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentPauseKey = EKeys::Enter;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentPauseKey = EKeys::BackSpace;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentPauseKey = EKeys::Equals;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentPauseKey = EKeys::Hyphen;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentPauseKey = EKeys::Zero;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentPauseKey = EKeys::One;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentPauseKey = EKeys::Two;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentPauseKey = EKeys::Three;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentPauseKey = EKeys::Four;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentPauseKey = EKeys::Five;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentPauseKey = EKeys::Six;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentPauseKey = EKeys::Seven;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentPauseKey = EKeys::Eight;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentPauseKey = EKeys::Nine;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentPauseKey = EKeys::Tilde;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentPauseKey = EKeys::Home;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentPauseKey = EKeys::PageUp;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentPauseKey = EKeys::PageDown;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentPauseKey = EKeys::End;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentPauseKey = EKeys::LeftBracket;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentPauseKey = EKeys::RightBracket;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentPauseKey = EKeys::Slash;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentPauseKey = EKeys::Semicolon;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentPauseKey = EKeys::Colon;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentPauseKey = EKeys::Quote;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentPauseKey = EKeys::Comma;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentPauseKey = EKeys::Period;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentPauseKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentPauseKey = EKeys::RightParantheses;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentPauseKey = EKeys::Escape;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentPauseKey = EKeys::Underscore;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentPauseKey = EKeys::F1;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentPauseKey = EKeys::F2;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentPauseKey = EKeys::F3;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentPauseKey = EKeys::F4;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentPauseKey = EKeys::F5;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentPauseKey = EKeys::F6;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentPauseKey = EKeys::F7;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentPauseKey = EKeys::F8;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentPauseKey = EKeys::F9;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentPauseKey = EKeys::F10;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentPauseKey = EKeys::F11;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentPauseKey = EKeys::F12;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentPauseKey = EKeys::Left;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentPauseKey = EKeys::Right;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentPauseKey = EKeys::Up;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentPauseKey = EKeys::Down;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentPauseKey = EKeys::Tab;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentPauseKey = EKeys::Delete;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentPauseKey = EKeys::RightAlt;
			APlayerCharacter::modifyPauseKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyPauseKey = false;
		}
	}
}

void AMainMenuActor::ChangeAttackKey()
{
	if (APlayerCharacter::modifyAttackKey)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			APlayerCharacter::currentAttackKey = EKeys::W;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			APlayerCharacter::currentAttackKey = EKeys::S;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			APlayerCharacter::currentAttackKey = EKeys::A;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			APlayerCharacter::currentAttackKey = EKeys::D;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
		{
			APlayerCharacter::currentAttackKey = EKeys::Q;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
		{
			APlayerCharacter::currentAttackKey = EKeys::E;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
		{
			APlayerCharacter::currentAttackKey = EKeys::R;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::T))
		{
			APlayerCharacter::currentAttackKey = EKeys::T;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
		{
			APlayerCharacter::currentAttackKey = EKeys::Y;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
		{
			APlayerCharacter::currentAttackKey = EKeys::U;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
		{
			APlayerCharacter::currentAttackKey = EKeys::I;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
		{
			APlayerCharacter::currentAttackKey = EKeys::O;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
		{
			APlayerCharacter::currentAttackKey = EKeys::P;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F))
		{
			APlayerCharacter::currentAttackKey = EKeys::F;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
		{
			APlayerCharacter::currentAttackKey = EKeys::G;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::H))
		{
			APlayerCharacter::currentAttackKey = EKeys::H;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
		{
			APlayerCharacter::currentAttackKey = EKeys::J;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::K))
		{
			APlayerCharacter::currentAttackKey = EKeys::K;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
		{
			APlayerCharacter::currentAttackKey = EKeys::L;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Z))
		{
			APlayerCharacter::currentAttackKey = EKeys::Z;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
		{
			APlayerCharacter::currentAttackKey = EKeys::X;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
		{
			APlayerCharacter::currentAttackKey = EKeys::C;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
		{
			APlayerCharacter::currentAttackKey = EKeys::V;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::B))
		{
			APlayerCharacter::currentAttackKey = EKeys::B;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
		{
			APlayerCharacter::currentAttackKey = EKeys::N;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
		{
			APlayerCharacter::currentAttackKey = EKeys::M;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::CapsLock))
		{
			APlayerCharacter::currentAttackKey = EKeys::CapsLock;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftShift))
		{
			APlayerCharacter::currentAttackKey = EKeys::LeftShift;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftControl))
		{
			APlayerCharacter::currentAttackKey = EKeys::LeftControl;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftAlt))
		{
			APlayerCharacter::currentAttackKey = EKeys::LeftAlt;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			APlayerCharacter::currentAttackKey = EKeys::SpaceBar;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Backslash))
		{
			APlayerCharacter::currentAttackKey = EKeys::Backslash;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightControl))
		{
			APlayerCharacter::currentAttackKey = EKeys::RightControl;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightShift))
		{
			APlayerCharacter::currentAttackKey = EKeys::RightShift;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
		{
			APlayerCharacter::currentAttackKey = EKeys::Enter;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			APlayerCharacter::currentAttackKey = EKeys::BackSpace;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Equals))
		{
			APlayerCharacter::currentAttackKey = EKeys::Equals;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Hyphen))
		{
			APlayerCharacter::currentAttackKey = EKeys::Hyphen;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
		{
			APlayerCharacter::currentAttackKey = EKeys::Zero;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
		{
			APlayerCharacter::currentAttackKey = EKeys::One;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		{
			APlayerCharacter::currentAttackKey = EKeys::Two;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
		{
			APlayerCharacter::currentAttackKey = EKeys::Three;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
		{
			APlayerCharacter::currentAttackKey = EKeys::Four;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
		{
			APlayerCharacter::currentAttackKey = EKeys::Five;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
		{
			APlayerCharacter::currentAttackKey = EKeys::Six;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
		{
			APlayerCharacter::currentAttackKey = EKeys::Seven;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
		{
			APlayerCharacter::currentAttackKey = EKeys::Eight;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
		{
			APlayerCharacter::currentAttackKey = EKeys::Nine;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tilde))
		{
			APlayerCharacter::currentAttackKey = EKeys::Tilde;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Home))
		{
			APlayerCharacter::currentAttackKey = EKeys::Home;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageUp))
		{
			APlayerCharacter::currentAttackKey = EKeys::PageUp;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::PageDown))
		{
			APlayerCharacter::currentAttackKey = EKeys::PageDown;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::End))
		{
			APlayerCharacter::currentAttackKey = EKeys::End;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			APlayerCharacter::currentAttackKey = EKeys::LeftBracket;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			APlayerCharacter::currentAttackKey = EKeys::RightBracket;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Slash))
		{
			APlayerCharacter::currentAttackKey = EKeys::Slash;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			APlayerCharacter::currentAttackKey = EKeys::Semicolon;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Colon))
		{
			APlayerCharacter::currentAttackKey = EKeys::Colon;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Quote))
		{
			APlayerCharacter::currentAttackKey = EKeys::Quote;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Comma))
		{
			APlayerCharacter::currentAttackKey = EKeys::Comma;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Period))
		{
			APlayerCharacter::currentAttackKey = EKeys::Period;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftParantheses))
		{
			APlayerCharacter::currentAttackKey = EKeys::LeftParantheses;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightParantheses))
		{
			APlayerCharacter::currentAttackKey = EKeys::RightParantheses;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape))
		{
			APlayerCharacter::currentAttackKey = EKeys::Escape;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Underscore))
		{
			APlayerCharacter::currentAttackKey = EKeys::Underscore;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
		{
			APlayerCharacter::currentAttackKey = EKeys::F1;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F2))
		{
			APlayerCharacter::currentAttackKey = EKeys::F2;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F3))
		{
			APlayerCharacter::currentAttackKey = EKeys::F3;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F4))
		{
			APlayerCharacter::currentAttackKey = EKeys::F4;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F5))
		{
			APlayerCharacter::currentAttackKey = EKeys::F5;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F6))
		{
			APlayerCharacter::currentAttackKey = EKeys::F6;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F7))
		{
			APlayerCharacter::currentAttackKey = EKeys::F7;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F8))
		{
			APlayerCharacter::currentAttackKey = EKeys::F8;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F9))
		{
			APlayerCharacter::currentAttackKey = EKeys::F9;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F10))
		{
			APlayerCharacter::currentAttackKey = EKeys::F10;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F11))
		{
			APlayerCharacter::currentAttackKey = EKeys::F11;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F12))
		{
			APlayerCharacter::currentAttackKey = EKeys::F12;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
		{
			APlayerCharacter::currentAttackKey = EKeys::Left;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
		{
			APlayerCharacter::currentAttackKey = EKeys::Right;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
		{
			APlayerCharacter::currentAttackKey = EKeys::Up;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
		{
			APlayerCharacter::currentAttackKey = EKeys::Down;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
		{
			APlayerCharacter::currentAttackKey = EKeys::Tab;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Delete))
		{
			APlayerCharacter::currentAttackKey = EKeys::Delete;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightAlt))
		{
			APlayerCharacter::currentAttackKey = EKeys::RightAlt;
			APlayerCharacter::modifyAttackKey = false;
		}

		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MiddleMouseButton) ||
			GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			APlayerCharacter::modifyAttackKey = false;
		}
	}
}