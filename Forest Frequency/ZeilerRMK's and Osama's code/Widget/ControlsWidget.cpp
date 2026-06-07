// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ControlsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/WidgetManager.h"
#include "Widget/MainMenuWidget.h"
#include "DefaultPlayerController.h"

UControlsWidget::UControlsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	defaultPlayerController = Cast<ADefaultPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!defaultPlayerController) return;

	// Update throttle input text
	UpdateInputText(MoveLeftInputText, defaultPlayerController->MoveLeftKey());

	// Update brake input text
	UpdateInputText(MoveRightInputText, defaultPlayerController->MoveRightKey());

	// Update steer left input text
	UpdateInputText(MoveDownInputText, defaultPlayerController->MoveDownKey());

	// Update steer right input text
	UpdateInputText(MoveUpInputText, defaultPlayerController->MoveUpKey());

	// Update handbrake input text
	UpdateInputText(JumpInputText, defaultPlayerController->JumpKey());

	// Update respawn input text
	UpdateInputText(InteractInputText, defaultPlayerController->InteractKey());

	// Update join race input text
	UpdateInputText(SprintInputText, defaultPlayerController->SprintKey());

	// Update pause game input text
	UpdateInputText(FireInputText, defaultPlayerController->FireKey());

	// Update career stats input text
	UpdateInputText(PauseInputText, defaultPlayerController->PauseKey());

	if (MoveLeftInputButton)
	{
		MoveLeftInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeMoveLeftInput);
	}

	if (MoveRightInputButton)
	{
		MoveRightInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeMoveRightInput);
	}

	if (MoveDownInputButton)
	{
		MoveDownInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeMoveDownInput);
	}

	if (MoveUpInputButton)
	{
		MoveUpInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeMoveUpInput);
	}

	if (JumpInputButton)
	{
		JumpInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeJumpInput);
	}

	if (InteractInputButton)
	{
		InteractInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeInteractInput);
	}

	if (SprintInputButton)
	{
		SprintInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeSprintInput);

		normalButtonColor = SprintInputButton->GetBackgroundColor();
	}

	if (FireInputButton)
	{
		FireInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangeFireInput);
	}

	if (PauseInputButton)
	{
		PauseInputButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::ChangePauseInput);
	}

	if (BackButton) BackButton->OnPressed.AddUniqueDynamic(this, &UControlsWidget::BackToMainMenu);
}

void UControlsWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	switch (mappingControlsState)
	{
	case MappingControlsState::EModifyingMoveLeftBinding:

		SetInputForMoveLeft(EKeys::A);
		SetInputForMoveLeft(EKeys::B);
		SetInputForMoveLeft(EKeys::C);
		SetInputForMoveLeft(EKeys::D);
		SetInputForMoveLeft(EKeys::E);
		SetInputForMoveLeft(EKeys::F);
		SetInputForMoveLeft(EKeys::G);
		SetInputForMoveLeft(EKeys::H);
		SetInputForMoveLeft(EKeys::I);
		SetInputForMoveLeft(EKeys::J);
		SetInputForMoveLeft(EKeys::K);
		SetInputForMoveLeft(EKeys::L);
		SetInputForMoveLeft(EKeys::M);
		SetInputForMoveLeft(EKeys::N);
		SetInputForMoveLeft(EKeys::O);
		SetInputForMoveLeft(EKeys::P);
		SetInputForMoveLeft(EKeys::Q);
		SetInputForMoveLeft(EKeys::R);
		SetInputForMoveLeft(EKeys::S);
		SetInputForMoveLeft(EKeys::T);
		SetInputForMoveLeft(EKeys::U);
		SetInputForMoveLeft(EKeys::V);
		SetInputForMoveLeft(EKeys::W);
		SetInputForMoveLeft(EKeys::X);
		SetInputForMoveLeft(EKeys::Y);
		SetInputForMoveLeft(EKeys::Z);

		SetInputForMoveLeft(EKeys::LeftShift);
		SetInputForMoveLeft(EKeys::RightShift);
		SetInputForMoveLeft(EKeys::LeftControl);
		SetInputForMoveLeft(EKeys::RightControl);
		SetInputForMoveLeft(EKeys::LeftAlt);
		SetInputForMoveLeft(EKeys::RightAlt);

		SetInputForMoveLeft(EKeys::Tab);
		SetInputForMoveLeft(EKeys::CapsLock);
		SetInputForMoveLeft(EKeys::BackSpace);
		SetInputForMoveLeft(EKeys::Enter);
		SetInputForMoveLeft(EKeys::Escape);
		SetInputForMoveLeft(EKeys::Tilde);
		SetInputForMoveLeft(EKeys::SpaceBar);

		SetInputForMoveLeft(EKeys::Hyphen);
		SetInputForMoveLeft(EKeys::Comma);
		SetInputForMoveLeft(EKeys::Period);
		SetInputForMoveLeft(EKeys::Slash);
		SetInputForMoveLeft(EKeys::Backslash);
		SetInputForMoveLeft(EKeys::Apostrophe);
		SetInputForMoveLeft(EKeys::Quote);
		SetInputForMoveLeft(EKeys::Semicolon);
		SetInputForMoveLeft(EKeys::Colon);
		SetInputForMoveLeft(EKeys::LeftBracket);
		SetInputForMoveLeft(EKeys::RightBracket);
		SetInputForMoveLeft(EKeys::Underscore);
		SetInputForMoveLeft(EKeys::Equals);
		SetInputForMoveLeft(EKeys::Add);
		SetInputForMoveLeft(EKeys::Subtract);

		SetInputForMoveLeft(EKeys::Delete);
		SetInputForMoveLeft(EKeys::Insert);
		SetInputForMoveLeft(EKeys::Home);
		SetInputForMoveLeft(EKeys::PageUp);
		SetInputForMoveLeft(EKeys::PageDown);
		SetInputForMoveLeft(EKeys::End);
		SetInputForMoveLeft(EKeys::ScrollLock);
		SetInputForMoveLeft(EKeys::NumLock);

		SetInputForMoveLeft(EKeys::F1);
		SetInputForMoveLeft(EKeys::F2);
		SetInputForMoveLeft(EKeys::F3);
		SetInputForMoveLeft(EKeys::F4);
		SetInputForMoveLeft(EKeys::F5);
		SetInputForMoveLeft(EKeys::F6);
		SetInputForMoveLeft(EKeys::F7);
		SetInputForMoveLeft(EKeys::F8);
		SetInputForMoveLeft(EKeys::F9);
		SetInputForMoveLeft(EKeys::F10);
		SetInputForMoveLeft(EKeys::F11);
		SetInputForMoveLeft(EKeys::F12);

		SetInputForMoveLeft(EKeys::Exclamation);
		SetInputForMoveLeft(EKeys::Dollar);
		SetInputForMoveLeft(EKeys::Ampersand);
		SetInputForMoveLeft(EKeys::LeftParantheses);
		SetInputForMoveLeft(EKeys::RightParantheses);
		SetInputForMoveLeft(EKeys::Caret);
		SetInputForMoveLeft(EKeys::Asterix);

		// Number keys
		SetInputForMoveLeft(EKeys::One);
		SetInputForMoveLeft(EKeys::Two);
		SetInputForMoveLeft(EKeys::Three);
		SetInputForMoveLeft(EKeys::Four);
		SetInputForMoveLeft(EKeys::Five);
		SetInputForMoveLeft(EKeys::Six);
		SetInputForMoveLeft(EKeys::Seven);
		SetInputForMoveLeft(EKeys::Eight);
		SetInputForMoveLeft(EKeys::Nine);
		SetInputForMoveLeft(EKeys::Zero);

		// Numpad keys
		SetInputForMoveLeft(EKeys::NumPadOne);
		SetInputForMoveLeft(EKeys::NumPadTwo);
		SetInputForMoveLeft(EKeys::NumPadThree);
		SetInputForMoveLeft(EKeys::NumPadFour);
		SetInputForMoveLeft(EKeys::NumPadFive);
		SetInputForMoveLeft(EKeys::NumPadSix);
		SetInputForMoveLeft(EKeys::NumPadSeven);
		SetInputForMoveLeft(EKeys::NumPadEight);
		SetInputForMoveLeft(EKeys::NumPadNine);
		SetInputForMoveLeft(EKeys::NumPadZero);

		// Arrow keys
		SetInputForMoveLeft(EKeys::Up);
		SetInputForMoveLeft(EKeys::Down);
		SetInputForMoveLeft(EKeys::Left);
		SetInputForMoveLeft(EKeys::Right);

		// Mouse buttons
		SetInputForMoveLeft(EKeys::LeftMouseButton);
		SetInputForMoveLeft(EKeys::MiddleMouseButton);
		SetInputForMoveLeft(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingMoveRightBinding:

		SetInputForMoveRight(EKeys::A);
		SetInputForMoveRight(EKeys::B);
		SetInputForMoveRight(EKeys::C);
		SetInputForMoveRight(EKeys::D);
		SetInputForMoveRight(EKeys::E);
		SetInputForMoveRight(EKeys::F);
		SetInputForMoveRight(EKeys::G);
		SetInputForMoveRight(EKeys::H);
		SetInputForMoveRight(EKeys::I);
		SetInputForMoveRight(EKeys::J);
		SetInputForMoveRight(EKeys::K);
		SetInputForMoveRight(EKeys::L);
		SetInputForMoveRight(EKeys::M);
		SetInputForMoveRight(EKeys::N);
		SetInputForMoveRight(EKeys::O);
		SetInputForMoveRight(EKeys::P);
		SetInputForMoveRight(EKeys::Q);
		SetInputForMoveRight(EKeys::R);
		SetInputForMoveRight(EKeys::S);
		SetInputForMoveRight(EKeys::T);
		SetInputForMoveRight(EKeys::U);
		SetInputForMoveRight(EKeys::V);
		SetInputForMoveRight(EKeys::W);
		SetInputForMoveRight(EKeys::X);
		SetInputForMoveRight(EKeys::Y);
		SetInputForMoveRight(EKeys::Z);

		SetInputForMoveRight(EKeys::LeftShift);
		SetInputForMoveRight(EKeys::RightShift);
		SetInputForMoveRight(EKeys::LeftControl);
		SetInputForMoveRight(EKeys::RightControl);
		SetInputForMoveRight(EKeys::LeftAlt);
		SetInputForMoveRight(EKeys::RightAlt);

		SetInputForMoveRight(EKeys::Tab);
		SetInputForMoveRight(EKeys::CapsLock);
		SetInputForMoveRight(EKeys::BackSpace);
		SetInputForMoveRight(EKeys::Enter);
		SetInputForMoveRight(EKeys::Escape);
		SetInputForMoveRight(EKeys::Tilde);
		SetInputForMoveRight(EKeys::SpaceBar);

		SetInputForMoveRight(EKeys::Hyphen);
		SetInputForMoveRight(EKeys::Comma);
		SetInputForMoveRight(EKeys::Period);
		SetInputForMoveRight(EKeys::Slash);
		SetInputForMoveRight(EKeys::Backslash);
		SetInputForMoveRight(EKeys::Apostrophe);
		SetInputForMoveRight(EKeys::Quote);
		SetInputForMoveRight(EKeys::Semicolon);
		SetInputForMoveRight(EKeys::Colon);
		SetInputForMoveRight(EKeys::LeftBracket);
		SetInputForMoveRight(EKeys::RightBracket);
		SetInputForMoveRight(EKeys::Underscore);
		SetInputForMoveRight(EKeys::Equals);
		SetInputForMoveRight(EKeys::Add);
		SetInputForMoveRight(EKeys::Subtract);

		SetInputForMoveRight(EKeys::Delete);
		SetInputForMoveRight(EKeys::Insert);
		SetInputForMoveRight(EKeys::Home);
		SetInputForMoveRight(EKeys::PageUp);
		SetInputForMoveRight(EKeys::PageDown);
		SetInputForMoveRight(EKeys::End);
		SetInputForMoveRight(EKeys::ScrollLock);
		SetInputForMoveRight(EKeys::NumLock);

		SetInputForMoveRight(EKeys::F1);
		SetInputForMoveRight(EKeys::F2);
		SetInputForMoveRight(EKeys::F3);
		SetInputForMoveRight(EKeys::F4);
		SetInputForMoveRight(EKeys::F5);
		SetInputForMoveRight(EKeys::F6);
		SetInputForMoveRight(EKeys::F7);
		SetInputForMoveRight(EKeys::F8);
		SetInputForMoveRight(EKeys::F9);
		SetInputForMoveRight(EKeys::F10);
		SetInputForMoveRight(EKeys::F11);
		SetInputForMoveRight(EKeys::F12);

		SetInputForMoveRight(EKeys::Exclamation);
		SetInputForMoveRight(EKeys::Dollar);
		SetInputForMoveRight(EKeys::Ampersand);
		SetInputForMoveRight(EKeys::LeftParantheses);
		SetInputForMoveRight(EKeys::RightParantheses);
		SetInputForMoveRight(EKeys::Caret);
		SetInputForMoveRight(EKeys::Asterix);

		// Number keys
		SetInputForMoveRight(EKeys::One);
		SetInputForMoveRight(EKeys::Two);
		SetInputForMoveRight(EKeys::Three);
		SetInputForMoveRight(EKeys::Four);
		SetInputForMoveRight(EKeys::Five);
		SetInputForMoveRight(EKeys::Six);
		SetInputForMoveRight(EKeys::Seven);
		SetInputForMoveRight(EKeys::Eight);
		SetInputForMoveRight(EKeys::Nine);
		SetInputForMoveRight(EKeys::Zero);

		// Numpad keys
		SetInputForMoveRight(EKeys::NumPadOne);
		SetInputForMoveRight(EKeys::NumPadTwo);
		SetInputForMoveRight(EKeys::NumPadThree);
		SetInputForMoveRight(EKeys::NumPadFour);
		SetInputForMoveRight(EKeys::NumPadFive);
		SetInputForMoveRight(EKeys::NumPadSix);
		SetInputForMoveRight(EKeys::NumPadSeven);
		SetInputForMoveRight(EKeys::NumPadEight);
		SetInputForMoveRight(EKeys::NumPadNine);
		SetInputForMoveRight(EKeys::NumPadZero);

		// Arrow keys
		SetInputForMoveRight(EKeys::Up);
		SetInputForMoveRight(EKeys::Down);
		SetInputForMoveRight(EKeys::Left);
		SetInputForMoveRight(EKeys::Right);

		// Mouse buttons
		SetInputForMoveRight(EKeys::LeftMouseButton);
		SetInputForMoveRight(EKeys::MiddleMouseButton);
		SetInputForMoveRight(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingMoveDownBinding:

		SetInputForMoveDown(EKeys::A);
		SetInputForMoveDown(EKeys::B);
		SetInputForMoveDown(EKeys::C);
		SetInputForMoveDown(EKeys::D);
		SetInputForMoveDown(EKeys::E);
		SetInputForMoveDown(EKeys::F);
		SetInputForMoveDown(EKeys::G);
		SetInputForMoveDown(EKeys::H);
		SetInputForMoveDown(EKeys::I);
		SetInputForMoveDown(EKeys::J);
		SetInputForMoveDown(EKeys::K);
		SetInputForMoveDown(EKeys::L);
		SetInputForMoveDown(EKeys::M);
		SetInputForMoveDown(EKeys::N);
		SetInputForMoveDown(EKeys::O);
		SetInputForMoveDown(EKeys::P);
		SetInputForMoveDown(EKeys::Q);
		SetInputForMoveDown(EKeys::R);
		SetInputForMoveDown(EKeys::S);
		SetInputForMoveDown(EKeys::T);
		SetInputForMoveDown(EKeys::U);
		SetInputForMoveDown(EKeys::V);
		SetInputForMoveDown(EKeys::W);
		SetInputForMoveDown(EKeys::X);
		SetInputForMoveDown(EKeys::Y);
		SetInputForMoveDown(EKeys::Z);

		SetInputForMoveDown(EKeys::LeftShift);
		SetInputForMoveDown(EKeys::RightShift);
		SetInputForMoveDown(EKeys::LeftControl);
		SetInputForMoveDown(EKeys::RightControl);
		SetInputForMoveDown(EKeys::LeftAlt);
		SetInputForMoveDown(EKeys::RightAlt);

		SetInputForMoveDown(EKeys::Tab);
		SetInputForMoveDown(EKeys::CapsLock);
		SetInputForMoveDown(EKeys::BackSpace);
		SetInputForMoveDown(EKeys::Enter);
		SetInputForMoveDown(EKeys::Escape);
		SetInputForMoveDown(EKeys::Tilde);
		SetInputForMoveDown(EKeys::SpaceBar);

		SetInputForMoveDown(EKeys::Hyphen);
		SetInputForMoveDown(EKeys::Comma);
		SetInputForMoveDown(EKeys::Period);
		SetInputForMoveDown(EKeys::Slash);
		SetInputForMoveDown(EKeys::Backslash);
		SetInputForMoveDown(EKeys::Apostrophe);
		SetInputForMoveDown(EKeys::Quote);
		SetInputForMoveDown(EKeys::Semicolon);
		SetInputForMoveDown(EKeys::Colon);
		SetInputForMoveDown(EKeys::LeftBracket);
		SetInputForMoveDown(EKeys::RightBracket);
		SetInputForMoveDown(EKeys::Underscore);
		SetInputForMoveDown(EKeys::Equals);
		SetInputForMoveDown(EKeys::Add);
		SetInputForMoveDown(EKeys::Subtract);

		SetInputForMoveDown(EKeys::Delete);
		SetInputForMoveDown(EKeys::Insert);
		SetInputForMoveDown(EKeys::Home);
		SetInputForMoveDown(EKeys::PageUp);
		SetInputForMoveDown(EKeys::PageDown);
		SetInputForMoveDown(EKeys::End);
		SetInputForMoveDown(EKeys::ScrollLock);
		SetInputForMoveDown(EKeys::NumLock);

		SetInputForMoveDown(EKeys::F1);
		SetInputForMoveDown(EKeys::F2);
		SetInputForMoveDown(EKeys::F3);
		SetInputForMoveDown(EKeys::F4);
		SetInputForMoveDown(EKeys::F5);
		SetInputForMoveDown(EKeys::F6);
		SetInputForMoveDown(EKeys::F7);
		SetInputForMoveDown(EKeys::F8);
		SetInputForMoveDown(EKeys::F9);
		SetInputForMoveDown(EKeys::F10);
		SetInputForMoveDown(EKeys::F11);
		SetInputForMoveDown(EKeys::F12);

		SetInputForMoveDown(EKeys::Exclamation);
		SetInputForMoveDown(EKeys::Dollar);
		SetInputForMoveDown(EKeys::Ampersand);
		SetInputForMoveDown(EKeys::LeftParantheses);
		SetInputForMoveDown(EKeys::RightParantheses);
		SetInputForMoveDown(EKeys::Caret);
		SetInputForMoveDown(EKeys::Asterix);

		// Number keys
		SetInputForMoveDown(EKeys::One);
		SetInputForMoveDown(EKeys::Two);
		SetInputForMoveDown(EKeys::Three);
		SetInputForMoveDown(EKeys::Four);
		SetInputForMoveDown(EKeys::Five);
		SetInputForMoveDown(EKeys::Six);
		SetInputForMoveDown(EKeys::Seven);
		SetInputForMoveDown(EKeys::Eight);
		SetInputForMoveDown(EKeys::Nine);
		SetInputForMoveDown(EKeys::Zero);

		// Numpad keys
		SetInputForMoveDown(EKeys::NumPadOne);
		SetInputForMoveDown(EKeys::NumPadTwo);
		SetInputForMoveDown(EKeys::NumPadThree);
		SetInputForMoveDown(EKeys::NumPadFour);
		SetInputForMoveDown(EKeys::NumPadFive);
		SetInputForMoveDown(EKeys::NumPadSix);
		SetInputForMoveDown(EKeys::NumPadSeven);
		SetInputForMoveDown(EKeys::NumPadEight);
		SetInputForMoveDown(EKeys::NumPadNine);
		SetInputForMoveDown(EKeys::NumPadZero);

		// Arrow keys
		SetInputForMoveDown(EKeys::Up);
		SetInputForMoveDown(EKeys::Down);
		SetInputForMoveDown(EKeys::Left);
		SetInputForMoveDown(EKeys::Right);

		// Mouse buttons
		SetInputForMoveDown(EKeys::LeftMouseButton);
		SetInputForMoveDown(EKeys::MiddleMouseButton);
		SetInputForMoveDown(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingMoveUpBinding:

		SetInputForMoveUp(EKeys::A);
		SetInputForMoveUp(EKeys::B);
		SetInputForMoveUp(EKeys::C);
		SetInputForMoveUp(EKeys::D);
		SetInputForMoveUp(EKeys::E);
		SetInputForMoveUp(EKeys::F);
		SetInputForMoveUp(EKeys::G);
		SetInputForMoveUp(EKeys::H);
		SetInputForMoveUp(EKeys::I);
		SetInputForMoveUp(EKeys::J);
		SetInputForMoveUp(EKeys::K);
		SetInputForMoveUp(EKeys::L);
		SetInputForMoveUp(EKeys::M);
		SetInputForMoveUp(EKeys::N);
		SetInputForMoveUp(EKeys::O);
		SetInputForMoveUp(EKeys::P);
		SetInputForMoveUp(EKeys::Q);
		SetInputForMoveUp(EKeys::R);
		SetInputForMoveUp(EKeys::S);
		SetInputForMoveUp(EKeys::T);
		SetInputForMoveUp(EKeys::U);
		SetInputForMoveUp(EKeys::V);
		SetInputForMoveUp(EKeys::W);
		SetInputForMoveUp(EKeys::X);
		SetInputForMoveUp(EKeys::Y);
		SetInputForMoveUp(EKeys::Z);

		SetInputForMoveUp(EKeys::LeftShift);
		SetInputForMoveUp(EKeys::RightShift);
		SetInputForMoveUp(EKeys::LeftControl);
		SetInputForMoveUp(EKeys::RightControl);
		SetInputForMoveUp(EKeys::LeftAlt);
		SetInputForMoveUp(EKeys::RightAlt);

		SetInputForMoveUp(EKeys::Tab);
		SetInputForMoveUp(EKeys::CapsLock);
		SetInputForMoveUp(EKeys::BackSpace);
		SetInputForMoveUp(EKeys::Enter);
		SetInputForMoveUp(EKeys::Escape);
		SetInputForMoveUp(EKeys::Tilde);
		SetInputForMoveUp(EKeys::SpaceBar);

		SetInputForMoveUp(EKeys::Hyphen);
		SetInputForMoveUp(EKeys::Comma);
		SetInputForMoveUp(EKeys::Period);
		SetInputForMoveUp(EKeys::Slash);
		SetInputForMoveUp(EKeys::Backslash);
		SetInputForMoveUp(EKeys::Apostrophe);
		SetInputForMoveUp(EKeys::Quote);
		SetInputForMoveUp(EKeys::Semicolon);
		SetInputForMoveUp(EKeys::Colon);
		SetInputForMoveUp(EKeys::LeftBracket);
		SetInputForMoveUp(EKeys::RightBracket);
		SetInputForMoveUp(EKeys::Underscore);
		SetInputForMoveUp(EKeys::Equals);
		SetInputForMoveUp(EKeys::Add);
		SetInputForMoveUp(EKeys::Subtract);

		SetInputForMoveUp(EKeys::Delete);
		SetInputForMoveUp(EKeys::Insert);
		SetInputForMoveUp(EKeys::Home);
		SetInputForMoveUp(EKeys::PageUp);
		SetInputForMoveUp(EKeys::PageDown);
		SetInputForMoveUp(EKeys::End);
		SetInputForMoveUp(EKeys::ScrollLock);
		SetInputForMoveUp(EKeys::NumLock);

		SetInputForMoveUp(EKeys::F1);
		SetInputForMoveUp(EKeys::F2);
		SetInputForMoveUp(EKeys::F3);
		SetInputForMoveUp(EKeys::F4);
		SetInputForMoveUp(EKeys::F5);
		SetInputForMoveUp(EKeys::F6);
		SetInputForMoveUp(EKeys::F7);
		SetInputForMoveUp(EKeys::F8);
		SetInputForMoveUp(EKeys::F9);
		SetInputForMoveUp(EKeys::F10);
		SetInputForMoveUp(EKeys::F11);
		SetInputForMoveUp(EKeys::F12);

		SetInputForMoveUp(EKeys::Exclamation);
		SetInputForMoveUp(EKeys::Dollar);
		SetInputForMoveUp(EKeys::Ampersand);
		SetInputForMoveUp(EKeys::LeftParantheses);
		SetInputForMoveUp(EKeys::RightParantheses);
		SetInputForMoveUp(EKeys::Caret);
		SetInputForMoveUp(EKeys::Asterix);

		// Number keys
		SetInputForMoveUp(EKeys::One);
		SetInputForMoveUp(EKeys::Two);
		SetInputForMoveUp(EKeys::Three);
		SetInputForMoveUp(EKeys::Four);
		SetInputForMoveUp(EKeys::Five);
		SetInputForMoveUp(EKeys::Six);
		SetInputForMoveUp(EKeys::Seven);
		SetInputForMoveUp(EKeys::Eight);
		SetInputForMoveUp(EKeys::Nine);
		SetInputForMoveUp(EKeys::Zero);

		// Numpad keys
		SetInputForMoveUp(EKeys::NumPadOne);
		SetInputForMoveUp(EKeys::NumPadTwo);
		SetInputForMoveUp(EKeys::NumPadThree);
		SetInputForMoveUp(EKeys::NumPadFour);
		SetInputForMoveUp(EKeys::NumPadFive);
		SetInputForMoveUp(EKeys::NumPadSix);
		SetInputForMoveUp(EKeys::NumPadSeven);
		SetInputForMoveUp(EKeys::NumPadEight);
		SetInputForMoveUp(EKeys::NumPadNine);
		SetInputForMoveUp(EKeys::NumPadZero);

		// Arrow keys
		SetInputForMoveUp(EKeys::Up);
		SetInputForMoveUp(EKeys::Down);
		SetInputForMoveUp(EKeys::Left);
		SetInputForMoveUp(EKeys::Right);

		// Mouse buttons
		SetInputForMoveUp(EKeys::LeftMouseButton);
		SetInputForMoveUp(EKeys::MiddleMouseButton);
		SetInputForMoveUp(EKeys::RightMouseButton);

		break;

	case MappingControlsState::EModifyingJumpBinding:

		SetInputForJump(EKeys::A);
		SetInputForJump(EKeys::B);
		SetInputForJump(EKeys::C);
		SetInputForJump(EKeys::D);
		SetInputForJump(EKeys::E);
		SetInputForJump(EKeys::F);
		SetInputForJump(EKeys::G);
		SetInputForJump(EKeys::H);
		SetInputForJump(EKeys::I);
		SetInputForJump(EKeys::J);
		SetInputForJump(EKeys::K);
		SetInputForJump(EKeys::L);
		SetInputForJump(EKeys::M);
		SetInputForJump(EKeys::N);
		SetInputForJump(EKeys::O);
		SetInputForJump(EKeys::P);
		SetInputForJump(EKeys::Q);
		SetInputForJump(EKeys::R);
		SetInputForJump(EKeys::S);
		SetInputForJump(EKeys::T);
		SetInputForJump(EKeys::U);
		SetInputForJump(EKeys::V);
		SetInputForJump(EKeys::W);
		SetInputForJump(EKeys::X);
		SetInputForJump(EKeys::Y);
		SetInputForJump(EKeys::Z);

		SetInputForJump(EKeys::LeftShift);
		SetInputForJump(EKeys::RightShift);
		SetInputForJump(EKeys::LeftControl);
		SetInputForJump(EKeys::RightControl);
		SetInputForJump(EKeys::LeftAlt);
		SetInputForJump(EKeys::RightAlt);

		SetInputForJump(EKeys::Tab);
		SetInputForJump(EKeys::CapsLock);
		SetInputForJump(EKeys::BackSpace);
		SetInputForJump(EKeys::Enter);
		SetInputForJump(EKeys::Escape);
		SetInputForJump(EKeys::Tilde);
		SetInputForJump(EKeys::SpaceBar);

		SetInputForJump(EKeys::Hyphen);
		SetInputForJump(EKeys::Comma);
		SetInputForJump(EKeys::Period);
		SetInputForJump(EKeys::Slash);
		SetInputForJump(EKeys::Backslash);
		SetInputForJump(EKeys::Apostrophe);
		SetInputForJump(EKeys::Quote);
		SetInputForJump(EKeys::Semicolon);
		SetInputForJump(EKeys::Colon);
		SetInputForJump(EKeys::LeftBracket);
		SetInputForJump(EKeys::RightBracket);
		SetInputForJump(EKeys::Underscore);
		SetInputForJump(EKeys::Equals);
		SetInputForJump(EKeys::Add);
		SetInputForJump(EKeys::Subtract);

		SetInputForJump(EKeys::Delete);
		SetInputForJump(EKeys::Insert);
		SetInputForJump(EKeys::Home);
		SetInputForJump(EKeys::PageUp);
		SetInputForJump(EKeys::PageDown);
		SetInputForJump(EKeys::End);
		SetInputForJump(EKeys::ScrollLock);
		SetInputForJump(EKeys::NumLock);

		SetInputForJump(EKeys::F1);
		SetInputForJump(EKeys::F2);
		SetInputForJump(EKeys::F3);
		SetInputForJump(EKeys::F4);
		SetInputForJump(EKeys::F5);
		SetInputForJump(EKeys::F6);
		SetInputForJump(EKeys::F7);
		SetInputForJump(EKeys::F8);
		SetInputForJump(EKeys::F9);
		SetInputForJump(EKeys::F10);
		SetInputForJump(EKeys::F11);
		SetInputForJump(EKeys::F12);

		SetInputForJump(EKeys::Exclamation);
		SetInputForJump(EKeys::Dollar);
		SetInputForJump(EKeys::Ampersand);
		SetInputForJump(EKeys::LeftParantheses);
		SetInputForJump(EKeys::RightParantheses);
		SetInputForJump(EKeys::Caret);
		SetInputForJump(EKeys::Asterix);

		// Number keys
		SetInputForJump(EKeys::One);
		SetInputForJump(EKeys::Two);
		SetInputForJump(EKeys::Three);
		SetInputForJump(EKeys::Four);
		SetInputForJump(EKeys::Five);
		SetInputForJump(EKeys::Six);
		SetInputForJump(EKeys::Seven);
		SetInputForJump(EKeys::Eight);
		SetInputForJump(EKeys::Nine);
		SetInputForJump(EKeys::Zero);

		// Numpad keys
		SetInputForJump(EKeys::NumPadOne);
		SetInputForJump(EKeys::NumPadTwo);
		SetInputForJump(EKeys::NumPadThree);
		SetInputForJump(EKeys::NumPadFour);
		SetInputForJump(EKeys::NumPadFive);
		SetInputForJump(EKeys::NumPadSix);
		SetInputForJump(EKeys::NumPadSeven);
		SetInputForJump(EKeys::NumPadEight);
		SetInputForJump(EKeys::NumPadNine);
		SetInputForJump(EKeys::NumPadZero);

		// Arrow keys
		SetInputForJump(EKeys::Up);
		SetInputForJump(EKeys::Down);
		SetInputForJump(EKeys::Left);
		SetInputForJump(EKeys::Right);

		// Mouse buttons
		SetInputForJump(EKeys::LeftMouseButton);
		SetInputForJump(EKeys::MiddleMouseButton);
		SetInputForJump(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingInteractBinding:

		SetInputForInteract(EKeys::A);
		SetInputForInteract(EKeys::B);
		SetInputForInteract(EKeys::C);
		SetInputForInteract(EKeys::D);
		SetInputForInteract(EKeys::E);
		SetInputForInteract(EKeys::F);
		SetInputForInteract(EKeys::G);
		SetInputForInteract(EKeys::H);
		SetInputForInteract(EKeys::I);
		SetInputForInteract(EKeys::J);
		SetInputForInteract(EKeys::K);
		SetInputForInteract(EKeys::L);
		SetInputForInteract(EKeys::M);
		SetInputForInteract(EKeys::N);
		SetInputForInteract(EKeys::O);
		SetInputForInteract(EKeys::P);
		SetInputForInteract(EKeys::Q);
		SetInputForInteract(EKeys::R);
		SetInputForInteract(EKeys::S);
		SetInputForInteract(EKeys::T);
		SetInputForInteract(EKeys::U);
		SetInputForInteract(EKeys::V);
		SetInputForInteract(EKeys::W);
		SetInputForInteract(EKeys::X);
		SetInputForInteract(EKeys::Y);
		SetInputForInteract(EKeys::Z);

		SetInputForInteract(EKeys::LeftShift);
		SetInputForInteract(EKeys::RightShift);
		SetInputForInteract(EKeys::LeftControl);
		SetInputForInteract(EKeys::RightControl);
		SetInputForInteract(EKeys::LeftAlt);
		SetInputForInteract(EKeys::RightAlt);

		SetInputForInteract(EKeys::Tab);
		SetInputForInteract(EKeys::CapsLock);
		SetInputForInteract(EKeys::BackSpace);
		SetInputForInteract(EKeys::Enter);
		SetInputForInteract(EKeys::Escape);
		SetInputForInteract(EKeys::Tilde);
		SetInputForInteract(EKeys::SpaceBar);

		SetInputForInteract(EKeys::Hyphen);
		SetInputForInteract(EKeys::Comma);
		SetInputForInteract(EKeys::Period);
		SetInputForInteract(EKeys::Slash);
		SetInputForInteract(EKeys::Backslash);
		SetInputForInteract(EKeys::Apostrophe);
		SetInputForInteract(EKeys::Quote);
		SetInputForInteract(EKeys::Semicolon);
		SetInputForInteract(EKeys::Colon);
		SetInputForInteract(EKeys::LeftBracket);
		SetInputForInteract(EKeys::RightBracket);
		SetInputForInteract(EKeys::Underscore);
		SetInputForInteract(EKeys::Equals);
		SetInputForInteract(EKeys::Add);
		SetInputForInteract(EKeys::Subtract);

		SetInputForInteract(EKeys::Delete);
		SetInputForInteract(EKeys::Insert);
		SetInputForInteract(EKeys::Home);
		SetInputForInteract(EKeys::PageUp);
		SetInputForInteract(EKeys::PageDown);
		SetInputForInteract(EKeys::End);
		SetInputForInteract(EKeys::ScrollLock);
		SetInputForInteract(EKeys::NumLock);

		SetInputForInteract(EKeys::F1);
		SetInputForInteract(EKeys::F2);
		SetInputForInteract(EKeys::F3);
		SetInputForInteract(EKeys::F4);
		SetInputForInteract(EKeys::F5);
		SetInputForInteract(EKeys::F6);
		SetInputForInteract(EKeys::F7);
		SetInputForInteract(EKeys::F8);
		SetInputForInteract(EKeys::F9);
		SetInputForInteract(EKeys::F10);
		SetInputForInteract(EKeys::F11);
		SetInputForInteract(EKeys::F12);

		SetInputForInteract(EKeys::Exclamation);
		SetInputForInteract(EKeys::Dollar);
		SetInputForInteract(EKeys::Ampersand);
		SetInputForInteract(EKeys::LeftParantheses);
		SetInputForInteract(EKeys::RightParantheses);
		SetInputForInteract(EKeys::Caret);
		SetInputForInteract(EKeys::Asterix);

		// Number keys
		SetInputForInteract(EKeys::One);
		SetInputForInteract(EKeys::Two);
		SetInputForInteract(EKeys::Three);
		SetInputForInteract(EKeys::Four);
		SetInputForInteract(EKeys::Five);
		SetInputForInteract(EKeys::Six);
		SetInputForInteract(EKeys::Seven);
		SetInputForInteract(EKeys::Eight);
		SetInputForInteract(EKeys::Nine);
		SetInputForInteract(EKeys::Zero);

		// Numpad keys
		SetInputForInteract(EKeys::NumPadOne);
		SetInputForInteract(EKeys::NumPadTwo);
		SetInputForInteract(EKeys::NumPadThree);
		SetInputForInteract(EKeys::NumPadFour);
		SetInputForInteract(EKeys::NumPadFive);
		SetInputForInteract(EKeys::NumPadSix);
		SetInputForInteract(EKeys::NumPadSeven);
		SetInputForInteract(EKeys::NumPadEight);
		SetInputForInteract(EKeys::NumPadNine);
		SetInputForInteract(EKeys::NumPadZero);

		// Arrow keys
		SetInputForInteract(EKeys::Up);
		SetInputForInteract(EKeys::Down);
		SetInputForInteract(EKeys::Left);
		SetInputForInteract(EKeys::Right);

		// Mouse buttons
		SetInputForInteract(EKeys::LeftMouseButton);
		SetInputForInteract(EKeys::MiddleMouseButton);
		SetInputForInteract(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingSprintBinding:

		SetInputForSprint(EKeys::A);
		SetInputForSprint(EKeys::B);
		SetInputForSprint(EKeys::C);
		SetInputForSprint(EKeys::D);
		SetInputForSprint(EKeys::E);
		SetInputForSprint(EKeys::F);
		SetInputForSprint(EKeys::G);
		SetInputForSprint(EKeys::H);
		SetInputForSprint(EKeys::I);
		SetInputForSprint(EKeys::J);
		SetInputForSprint(EKeys::K);
		SetInputForSprint(EKeys::L);
		SetInputForSprint(EKeys::M);
		SetInputForSprint(EKeys::N);
		SetInputForSprint(EKeys::O);
		SetInputForSprint(EKeys::P);
		SetInputForSprint(EKeys::Q);
		SetInputForSprint(EKeys::R);
		SetInputForSprint(EKeys::S);
		SetInputForSprint(EKeys::T);
		SetInputForSprint(EKeys::U);
		SetInputForSprint(EKeys::V);
		SetInputForSprint(EKeys::W);
		SetInputForSprint(EKeys::X);
		SetInputForSprint(EKeys::Y);
		SetInputForSprint(EKeys::Z);

		SetInputForSprint(EKeys::LeftShift);
		SetInputForSprint(EKeys::RightShift);
		SetInputForSprint(EKeys::LeftControl);
		SetInputForSprint(EKeys::RightControl);
		SetInputForSprint(EKeys::LeftAlt);
		SetInputForSprint(EKeys::RightAlt);

		SetInputForSprint(EKeys::Tab);
		SetInputForSprint(EKeys::CapsLock);
		SetInputForSprint(EKeys::BackSpace);
		SetInputForSprint(EKeys::Enter);
		SetInputForSprint(EKeys::Escape);
		SetInputForSprint(EKeys::Tilde);
		SetInputForSprint(EKeys::SpaceBar);

		SetInputForSprint(EKeys::Hyphen);
		SetInputForSprint(EKeys::Comma);
		SetInputForSprint(EKeys::Period);
		SetInputForSprint(EKeys::Slash);
		SetInputForSprint(EKeys::Backslash);
		SetInputForSprint(EKeys::Apostrophe);
		SetInputForSprint(EKeys::Quote);
		SetInputForSprint(EKeys::Semicolon);
		SetInputForSprint(EKeys::Colon);
		SetInputForSprint(EKeys::LeftBracket);
		SetInputForSprint(EKeys::RightBracket);
		SetInputForSprint(EKeys::Underscore);
		SetInputForSprint(EKeys::Equals);
		SetInputForSprint(EKeys::Add);
		SetInputForSprint(EKeys::Subtract);

		SetInputForSprint(EKeys::Delete);
		SetInputForSprint(EKeys::Insert);
		SetInputForSprint(EKeys::Home);
		SetInputForSprint(EKeys::PageUp);
		SetInputForSprint(EKeys::PageDown);
		SetInputForSprint(EKeys::End);
		SetInputForSprint(EKeys::ScrollLock);
		SetInputForSprint(EKeys::NumLock);

		SetInputForSprint(EKeys::F1);
		SetInputForSprint(EKeys::F2);
		SetInputForSprint(EKeys::F3);
		SetInputForSprint(EKeys::F4);
		SetInputForSprint(EKeys::F5);
		SetInputForSprint(EKeys::F6);
		SetInputForSprint(EKeys::F7);
		SetInputForSprint(EKeys::F8);
		SetInputForSprint(EKeys::F9);
		SetInputForSprint(EKeys::F10);
		SetInputForSprint(EKeys::F11);
		SetInputForSprint(EKeys::F12);

		SetInputForSprint(EKeys::Exclamation);
		SetInputForSprint(EKeys::Dollar);
		SetInputForSprint(EKeys::Ampersand);
		SetInputForSprint(EKeys::LeftParantheses);
		SetInputForSprint(EKeys::RightParantheses);
		SetInputForSprint(EKeys::Caret);
		SetInputForSprint(EKeys::Asterix);

		// Number keys
		SetInputForSprint(EKeys::One);
		SetInputForSprint(EKeys::Two);
		SetInputForSprint(EKeys::Three);
		SetInputForSprint(EKeys::Four);
		SetInputForSprint(EKeys::Five);
		SetInputForSprint(EKeys::Six);
		SetInputForSprint(EKeys::Seven);
		SetInputForSprint(EKeys::Eight);
		SetInputForSprint(EKeys::Nine);
		SetInputForSprint(EKeys::Zero);

		// Numpad keys
		SetInputForSprint(EKeys::NumPadOne);
		SetInputForSprint(EKeys::NumPadTwo);
		SetInputForSprint(EKeys::NumPadThree);
		SetInputForSprint(EKeys::NumPadFour);
		SetInputForSprint(EKeys::NumPadFive);
		SetInputForSprint(EKeys::NumPadSix);
		SetInputForSprint(EKeys::NumPadSeven);
		SetInputForSprint(EKeys::NumPadEight);
		SetInputForSprint(EKeys::NumPadNine);
		SetInputForSprint(EKeys::NumPadZero);

		// Arrow keys
		SetInputForSprint(EKeys::Up);
		SetInputForSprint(EKeys::Down);
		SetInputForSprint(EKeys::Left);
		SetInputForSprint(EKeys::Right);

		// Mouse buttons
		SetInputForSprint(EKeys::LeftMouseButton);
		SetInputForSprint(EKeys::MiddleMouseButton);
		SetInputForSprint(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingFireBinding:

		SetInputForFire(EKeys::A);
		SetInputForFire(EKeys::B);
		SetInputForFire(EKeys::C);
		SetInputForFire(EKeys::D);
		SetInputForFire(EKeys::E);
		SetInputForFire(EKeys::F);
		SetInputForFire(EKeys::G);
		SetInputForFire(EKeys::H);
		SetInputForFire(EKeys::I);
		SetInputForFire(EKeys::J);
		SetInputForFire(EKeys::K);
		SetInputForFire(EKeys::L);
		SetInputForFire(EKeys::M);
		SetInputForFire(EKeys::N);
		SetInputForFire(EKeys::O);
		SetInputForFire(EKeys::P);
		SetInputForFire(EKeys::Q);
		SetInputForFire(EKeys::R);
		SetInputForFire(EKeys::S);
		SetInputForFire(EKeys::T);
		SetInputForFire(EKeys::U);
		SetInputForFire(EKeys::V);
		SetInputForFire(EKeys::W);
		SetInputForFire(EKeys::X);
		SetInputForFire(EKeys::Y);
		SetInputForFire(EKeys::Z);

		SetInputForFire(EKeys::LeftShift);
		SetInputForFire(EKeys::RightShift);
		SetInputForFire(EKeys::LeftControl);
		SetInputForFire(EKeys::RightControl);
		SetInputForFire(EKeys::LeftAlt);
		SetInputForFire(EKeys::RightAlt);

		SetInputForFire(EKeys::Tab);
		SetInputForFire(EKeys::CapsLock);
		SetInputForFire(EKeys::BackSpace);
		SetInputForFire(EKeys::Enter);
		SetInputForFire(EKeys::Escape);
		SetInputForFire(EKeys::Tilde);
		SetInputForFire(EKeys::SpaceBar);

		SetInputForFire(EKeys::Hyphen);
		SetInputForFire(EKeys::Comma);
		SetInputForFire(EKeys::Period);
		SetInputForFire(EKeys::Slash);
		SetInputForFire(EKeys::Backslash);
		SetInputForFire(EKeys::Apostrophe);
		SetInputForFire(EKeys::Quote);
		SetInputForFire(EKeys::Semicolon);
		SetInputForFire(EKeys::Colon);
		SetInputForFire(EKeys::LeftBracket);
		SetInputForFire(EKeys::RightBracket);
		SetInputForFire(EKeys::Underscore);
		SetInputForFire(EKeys::Equals);
		SetInputForFire(EKeys::Add);
		SetInputForFire(EKeys::Subtract);

		SetInputForFire(EKeys::Delete);
		SetInputForFire(EKeys::Insert);
		SetInputForFire(EKeys::Home);
		SetInputForFire(EKeys::PageUp);
		SetInputForFire(EKeys::PageDown);
		SetInputForFire(EKeys::End);
		SetInputForFire(EKeys::ScrollLock);
		SetInputForFire(EKeys::NumLock);

		SetInputForFire(EKeys::F1);
		SetInputForFire(EKeys::F2);
		SetInputForFire(EKeys::F3);
		SetInputForFire(EKeys::F4);
		SetInputForFire(EKeys::F5);
		SetInputForFire(EKeys::F6);
		SetInputForFire(EKeys::F7);
		SetInputForFire(EKeys::F8);
		SetInputForFire(EKeys::F9);
		SetInputForFire(EKeys::F10);
		SetInputForFire(EKeys::F11);
		SetInputForFire(EKeys::F12);

		SetInputForFire(EKeys::Exclamation);
		SetInputForFire(EKeys::Dollar);
		SetInputForFire(EKeys::Ampersand);
		SetInputForFire(EKeys::LeftParantheses);
		SetInputForFire(EKeys::RightParantheses);
		SetInputForFire(EKeys::Caret);
		SetInputForFire(EKeys::Asterix);

		// Number keys
		SetInputForFire(EKeys::One);
		SetInputForFire(EKeys::Two);
		SetInputForFire(EKeys::Three);
		SetInputForFire(EKeys::Four);
		SetInputForFire(EKeys::Five);
		SetInputForFire(EKeys::Six);
		SetInputForFire(EKeys::Seven);
		SetInputForFire(EKeys::Eight);
		SetInputForFire(EKeys::Nine);
		SetInputForFire(EKeys::Zero);

		// Numpad keys
		SetInputForFire(EKeys::NumPadOne);
		SetInputForFire(EKeys::NumPadTwo);
		SetInputForFire(EKeys::NumPadThree);
		SetInputForFire(EKeys::NumPadFour);
		SetInputForFire(EKeys::NumPadFive);
		SetInputForFire(EKeys::NumPadSix);
		SetInputForFire(EKeys::NumPadSeven);
		SetInputForFire(EKeys::NumPadEight);
		SetInputForFire(EKeys::NumPadNine);
		SetInputForFire(EKeys::NumPadZero);

		// Arrow keys
		SetInputForFire(EKeys::Up);
		SetInputForFire(EKeys::Down);
		SetInputForFire(EKeys::Left);
		SetInputForFire(EKeys::Right);

		// Mouse buttons
		SetInputForFire(EKeys::LeftMouseButton);
		SetInputForFire(EKeys::MiddleMouseButton);
		SetInputForFire(EKeys::RightMouseButton);
		break;

	case MappingControlsState::EModifyingPauseBinding:

		SetInputForPause(EKeys::A);
		SetInputForPause(EKeys::B);
		SetInputForPause(EKeys::C);
		SetInputForPause(EKeys::D);
		SetInputForPause(EKeys::E);
		SetInputForPause(EKeys::F);
		SetInputForPause(EKeys::G);
		SetInputForPause(EKeys::H);
		SetInputForPause(EKeys::I);
		SetInputForPause(EKeys::J);
		SetInputForPause(EKeys::K);
		SetInputForPause(EKeys::L);
		SetInputForPause(EKeys::M);
		SetInputForPause(EKeys::N);
		SetInputForPause(EKeys::O);
		SetInputForPause(EKeys::P);
		SetInputForPause(EKeys::Q);
		SetInputForPause(EKeys::R);
		SetInputForPause(EKeys::S);
		SetInputForPause(EKeys::T);
		SetInputForPause(EKeys::U);
		SetInputForPause(EKeys::V);
		SetInputForPause(EKeys::W);
		SetInputForPause(EKeys::X);
		SetInputForPause(EKeys::Y);
		SetInputForPause(EKeys::Z);

		SetInputForPause(EKeys::LeftShift);
		SetInputForPause(EKeys::RightShift);
		SetInputForPause(EKeys::LeftControl);
		SetInputForPause(EKeys::RightControl);
		SetInputForPause(EKeys::LeftAlt);
		SetInputForPause(EKeys::RightAlt);

		SetInputForPause(EKeys::Tab);
		SetInputForPause(EKeys::CapsLock);
		SetInputForPause(EKeys::BackSpace);
		SetInputForPause(EKeys::Enter);
		SetInputForPause(EKeys::Escape);
		SetInputForPause(EKeys::Tilde);
		SetInputForPause(EKeys::SpaceBar);

		SetInputForPause(EKeys::Hyphen);
		SetInputForPause(EKeys::Comma);
		SetInputForPause(EKeys::Period);
		SetInputForPause(EKeys::Slash);
		SetInputForPause(EKeys::Backslash);
		SetInputForPause(EKeys::Apostrophe);
		SetInputForPause(EKeys::Quote);
		SetInputForPause(EKeys::Semicolon);
		SetInputForPause(EKeys::Colon);
		SetInputForPause(EKeys::LeftBracket);
		SetInputForPause(EKeys::RightBracket);
		SetInputForPause(EKeys::Underscore);
		SetInputForPause(EKeys::Equals);
		SetInputForPause(EKeys::Add);
		SetInputForPause(EKeys::Subtract);

		SetInputForPause(EKeys::Delete);
		SetInputForPause(EKeys::Insert);
		SetInputForPause(EKeys::Home);
		SetInputForPause(EKeys::PageUp);
		SetInputForPause(EKeys::PageDown);
		SetInputForPause(EKeys::End);
		SetInputForPause(EKeys::ScrollLock);
		SetInputForPause(EKeys::NumLock);

		SetInputForPause(EKeys::F1);
		SetInputForPause(EKeys::F2);
		SetInputForPause(EKeys::F3);
		SetInputForPause(EKeys::F4);
		SetInputForPause(EKeys::F5);
		SetInputForPause(EKeys::F6);
		SetInputForPause(EKeys::F7);
		SetInputForPause(EKeys::F8);
		SetInputForPause(EKeys::F9);
		SetInputForPause(EKeys::F10);
		SetInputForPause(EKeys::F11);
		SetInputForPause(EKeys::F12);

		SetInputForPause(EKeys::Exclamation);
		SetInputForPause(EKeys::Dollar);
		SetInputForPause(EKeys::Ampersand);
		SetInputForPause(EKeys::LeftParantheses);
		SetInputForPause(EKeys::RightParantheses);
		SetInputForPause(EKeys::Caret);
		SetInputForPause(EKeys::Asterix);

		// Number keys
		SetInputForPause(EKeys::One);
		SetInputForPause(EKeys::Two);
		SetInputForPause(EKeys::Three);
		SetInputForPause(EKeys::Four);
		SetInputForPause(EKeys::Five);
		SetInputForPause(EKeys::Six);
		SetInputForPause(EKeys::Seven);
		SetInputForPause(EKeys::Eight);
		SetInputForPause(EKeys::Nine);
		SetInputForPause(EKeys::Zero);

		// Numpad keys
		SetInputForPause(EKeys::NumPadOne);
		SetInputForPause(EKeys::NumPadTwo);
		SetInputForPause(EKeys::NumPadThree);
		SetInputForPause(EKeys::NumPadFour);
		SetInputForPause(EKeys::NumPadFive);
		SetInputForPause(EKeys::NumPadSix);
		SetInputForPause(EKeys::NumPadSeven);
		SetInputForPause(EKeys::NumPadEight);
		SetInputForPause(EKeys::NumPadNine);
		SetInputForPause(EKeys::NumPadZero);

		// Arrow keys
		SetInputForPause(EKeys::Up);
		SetInputForPause(EKeys::Down);
		SetInputForPause(EKeys::Left);
		SetInputForPause(EKeys::Right);

		// Mouse buttons
		SetInputForPause(EKeys::LeftMouseButton);
		SetInputForPause(EKeys::MiddleMouseButton);
		SetInputForPause(EKeys::RightMouseButton);
		break;

	default:
		break;
	}
}

void UControlsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (defaultPlayerController) defaultPlayerController = nullptr;

	if (MoveLeftInputButton)
	{
		MoveLeftInputButton->RemoveFromParent();
		MoveLeftInputButton = nullptr;
	}

	if (MoveLeftInputText)
	{
		MoveLeftInputText->RemoveFromParent();
		MoveLeftInputText = nullptr;
	}

	if (MoveLeftText)
	{
		MoveLeftText->RemoveFromParent();
		MoveLeftText = nullptr;
	}

	if (MoveRightInputButton)
	{
		MoveRightInputButton->RemoveFromParent();
		MoveRightInputButton = nullptr;
	}

	if (MoveRightInputText)
	{
		MoveRightInputText->RemoveFromParent();
		MoveRightInputText = nullptr;
	}

	if (MoveRightText)
	{
		MoveRightText->RemoveFromParent();
		MoveRightText = nullptr;
	}

	if (MoveDownInputButton)
	{
		MoveDownInputButton->RemoveFromParent();
		MoveDownInputButton = nullptr;
	}

	if (MoveDownInputText)
	{
		MoveDownInputText->RemoveFromParent();
		MoveDownInputText = nullptr;
	}

	if (MoveDownText)
	{
		MoveDownText->RemoveFromParent();
		MoveDownText = nullptr;
	}

	if (MoveUpInputButton)
	{
		MoveUpInputButton->RemoveFromParent();
		MoveUpInputButton = nullptr;
	}

	if (MoveUpInputText)
	{
		MoveUpInputText->RemoveFromParent();
		MoveUpInputText = nullptr;
	}

	if (MoveUpText)
	{
		MoveUpText->RemoveFromParent();
		MoveUpText = nullptr;
	}

	if (JumpInputButton)
	{
		JumpInputButton->RemoveFromParent();
		JumpInputButton = nullptr;
	}

	if (JumpInputText)
	{
		JumpInputText->RemoveFromParent();
		JumpInputText = nullptr;
	}

	if (JumpText)
	{
		JumpText->RemoveFromParent();
		JumpText = nullptr;
	}

	if (InteractInputButton)
	{
		InteractInputButton->RemoveFromParent();
		InteractInputButton = nullptr;
	}

	if (InteractInputText)
	{
		InteractInputText->RemoveFromParent();
		InteractInputText = nullptr;
	}

	if (InteractText)
	{
		InteractText->RemoveFromParent();
		InteractText = nullptr;
	}

	if (SprintInputButton)
	{
		SprintInputButton->RemoveFromParent();
		SprintInputButton = nullptr;
	}

	if (SprintInputText)
	{
		SprintInputText->RemoveFromParent();
		SprintInputText = nullptr;
	}

	if (SprintText)
	{
		SprintText->RemoveFromParent();
		SprintText = nullptr;
	}

	if (FireInputButton)
	{
		FireInputButton->RemoveFromParent();
		FireInputButton = nullptr;
	}

	if (FireInputText)
	{
		FireInputText->RemoveFromParent();
		FireInputText = nullptr;
	}

	if (FireText)
	{
		FireText->RemoveFromParent();
		FireText = nullptr;
	}

	if (PauseInputButton)
	{
		PauseInputButton->RemoveFromParent();
		PauseInputButton = nullptr;
	}

	if (PauseInputText)
	{
		PauseInputText->RemoveFromParent();
		PauseInputText = nullptr;
	}

	if (PauseText)
	{
		PauseText->RemoveFromParent();
		PauseText = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (HookImage)
	{
		HookImage->RemoveFromParent();
		HookImage = nullptr;
	}

	if (ControlsCanvas)
	{
		ControlsCanvas->RemoveFromParent();
		ControlsCanvas = nullptr;
	}
}

void UControlsWidget::ChangeMoveLeftInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (MoveLeftInputButton) MoveLeftInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingMoveLeftBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingMoveLeftBinding)
	{
		if (MoveLeftInputButton) MoveLeftInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeMoveRightInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (MoveRightInputButton) MoveRightInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingMoveRightBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingMoveRightBinding)
	{
		if (MoveRightInputButton) MoveRightInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeMoveDownInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (MoveDownInputButton) MoveDownInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingMoveDownBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingMoveDownBinding)
	{
		if (MoveDownInputButton) MoveDownInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeMoveUpInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (MoveUpInputButton) MoveUpInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingMoveUpBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingMoveUpBinding)
	{
		if (MoveUpInputButton) MoveUpInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeJumpInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (JumpInputButton) JumpInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingJumpBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingJumpBinding)
	{
		if (JumpInputButton) JumpInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeInteractInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (InteractInputButton) InteractInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingInteractBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingInteractBinding)
	{
		if (InteractInputButton) InteractInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeSprintInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (SprintInputButton) SprintInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingSprintBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingSprintBinding)
	{
		if (SprintInputButton) SprintInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangeFireInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (FireInputButton) FireInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingFireBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingFireBinding)
	{
		if (FireInputButton) FireInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::ChangePauseInput()
{
	if (mappingControlsState == MappingControlsState::ENull)
	{
		if (PauseInputButton) PauseInputButton->SetBackgroundColor(inputToPressButtonColor);
		mappingControlsState = MappingControlsState::EModifyingPauseBinding;
	}

	else if (mappingControlsState == MappingControlsState::EModifyingPauseBinding)
	{
		if (PauseInputButton) PauseInputButton->SetBackgroundColor(normalButtonColor);
		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::BackToMainMenu()
{
	if (mappingControlsState != MappingControlsState::ENull) return;

	RemoveBindedDelegates();

	if (!PressBackAnimation) return;

	PlayAnimation(PressBackAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UControlsWidget::TransitionToMainMenu,
		PressBackAnimation->GetEndTime(), false);
}

void UControlsWidget::TransitionToMainMenu()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetControlsClass());
	widgetManager->PushWidget(widgetManager->GetMainMenuClass());
}

void UControlsWidget::SetInputForMoveLeft(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateMoveLeftInput(newKey_);
			UpdateInputText(MoveLeftInputText, newKey_);
		}

		if (MoveLeftInputButton) MoveLeftInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForMoveRight(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateMoveRightInput(newKey_);
			UpdateInputText(MoveRightInputText, newKey_);
		}

		if (MoveRightInputButton) MoveRightInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForMoveDown(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateMoveDownInput(newKey_);
			UpdateInputText(MoveDownInputText, newKey_);
		}

		if (MoveDownInputButton) MoveDownInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForMoveUp(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateMoveUpInput(newKey_);
			UpdateInputText(MoveUpInputText, newKey_);
		}

		if (MoveUpInputButton) MoveUpInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForJump(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateJumpInput(newKey_);
			UpdateInputText(JumpInputText, newKey_);
		}

		if (JumpInputButton) JumpInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForInteract(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateInteractInput(newKey_);
			UpdateInputText(InteractInputText, newKey_);
		}

		if (InteractInputButton) InteractInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForSprint(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateSprintInput(newKey_);
			UpdateInputText(SprintInputText, newKey_);
		}

		if (SprintInputButton) SprintInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForFire(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdateFireInput(newKey_);
			UpdateInputText(FireInputText, newKey_);
		}

		if (FireInputButton) FireInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::SetInputForPause(const FKey& newKey_)
{
	if (defaultPlayerController && defaultPlayerController->IsInputKeyDown(newKey_))
	{
		if (!defaultPlayerController->NewKeyConflictsWithActionMapping(newKey_))
		{
			defaultPlayerController->UpdatePauseInput(newKey_);
			UpdateInputText(PauseInputText, newKey_);
		}

		if (PauseInputButton) PauseInputButton->SetBackgroundColor(normalButtonColor);

		mappingControlsState = MappingControlsState::ENull;
	}
}

void UControlsWidget::UpdateInputText(UTextBlock* textBlock_, const FKey& assignedKey_)
{
	if (textBlock_)
	{
		if (assignedKey_ == EKeys::LeftMouseButton)
		{
			textBlock_->SetText(FText::FromString("LeftMouse"));
		}

		else if (assignedKey_ == EKeys::MiddleMouseButton)
		{
			textBlock_->SetText(FText::FromString("MiddleMouse"));
		}

		else if (assignedKey_ == EKeys::RightMouseButton)
		{
			textBlock_->SetText(FText::FromString("RightMouse"));
		}

		else
		{
			textBlock_->SetText(FText::FromString(assignedKey_.ToString()));
		}
	}
}

void UControlsWidget::RemoveBindedDelegates()
{
	if (MoveLeftInputButton) MoveLeftInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeMoveLeftInput);
	if (MoveRightInputButton) MoveRightInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeMoveRightInput);
	if (MoveDownInputButton) MoveDownInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeMoveDownInput);
	if (MoveUpInputButton) MoveUpInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeMoveUpInput);
	if (JumpInputButton) JumpInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeJumpInput);
	if (InteractInputButton) InteractInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeInteractInput);
	if (SprintInputButton) SprintInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeSprintInput);
	if (FireInputButton) FireInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangeFireInput);
	if (PauseInputButton) PauseInputButton->OnPressed.RemoveDynamic(this, &UControlsWidget::ChangePauseInput);
	if (BackButton) BackButton->OnPressed.RemoveDynamic(this, &UControlsWidget::BackToMainMenu);
}
