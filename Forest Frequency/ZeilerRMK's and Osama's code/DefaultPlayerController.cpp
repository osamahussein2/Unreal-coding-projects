// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "PlayerMappableKeySettings.h"
#include "EnhancedInputLibrary.h"

#include "InputAction.h"
#include "InputMappingContext.h"

#include "DefaultPlayer.h"

ADefaultPlayerController::ADefaultPlayerController()
{
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultMappingContext) return;

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return;

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		Settings->RegisterInputMappingContext(DefaultMappingContext);
	}

	// Timer handle for updating control mappings
	FTimerHandle updateControlMappingHandle{};
	GetWorld()->GetTimerManager().SetTimer(updateControlMappingHandle, this, &ADefaultPlayerController::InitializeKeys,
		0.1f, false);
}

void ADefaultPlayerController::Tick(float DeltaTime)
{
}

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Move);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::None, this, &ADefaultPlayerController::SwingAround);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Jump);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Interact);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Sprint);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Fire);
		EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ADefaultPlayerController::Pause);
	}
}

void ADefaultPlayerController::InitializeKeys()
{
	if (MoveAction)
	{
		currentMoveLeftKey = MoveLeftKey();
		currentMoveRightKey = MoveRightKey();
		currentMoveDownKey = MoveDownKey();
		currentMoveUpKey = MoveUpKey();
	}

	if (JumpAction) currentJumpKey = JumpKey();
	if (InteractAction) currentInteractKey = InteractKey();
	if (SprintAction) currentSprintKey = SprintKey();
	if (FireAction) currentFireKey = FireKey();
	if (PauseAction) currentPauseKey = PauseKey();
}

void ADefaultPlayerController::Move(const FInputActionValue& Value)
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Move(Value);
	}
}

void ADefaultPlayerController::SwingAround()
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->SwingAround();
	}
}

void ADefaultPlayerController::Look(const FInputActionValue& Value)
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Look(Value);
	}
}

void ADefaultPlayerController::Interact(const FInputActionValue& Value)
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Interact(Value);
	}
}

void ADefaultPlayerController::Sprint(const FInputActionValue& Value)
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Sprint(Value);
	}
}

void ADefaultPlayerController::Fire(const FInputActionValue& Value)
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Fire(Value);
	}
}

void ADefaultPlayerController::Jump()
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Jump();
	}
}

void ADefaultPlayerController::Pause()
{
	if (ADefaultPlayer* defaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		defaultPlayer->Pause();
	}
}

FKey ADefaultPlayerController::MoveLeftKey() const
{
	if (!DefaultMappingContext || !MoveAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for steer, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(MoveAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == MoveAction &&
				DefaultMappingContext->GetMapping(i).GetMappingName() == moveLeftMapName)
			{
				return DefaultMappingContext->GetMapping(i).Key;
			}

			else continue;
		}
	}

	// If there is a key mapping for steer, use that key mapping
	else
	{
		// Query for keys mapped to steer input action
		for (int i = 0; i < enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings().Num(); ++i)
		{
			if (enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].GetMappingName() == moveLeftMapName)
				return enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::MoveRightKey() const
{
	if (!DefaultMappingContext || !MoveAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for steer, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(MoveAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == MoveAction &&
				DefaultMappingContext->GetMapping(i).GetMappingName() == moveRightMapName)
			{
				return DefaultMappingContext->GetMapping(i).Key;
			}

			else continue;
		}
	}

	// If there is a key mapping for steer, use that key mapping
	else
	{
		// Query for keys mapped to steer input action
		for (int i = 0; i < enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings().Num(); ++i)
		{
			if (enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].GetMappingName() == moveRightMapName)
				return enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::MoveDownKey() const
{
	if (!DefaultMappingContext || !MoveAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for steer, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(MoveAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == MoveAction &&
				DefaultMappingContext->GetMapping(i).GetMappingName() == moveDownMapName)
			{
				return DefaultMappingContext->GetMapping(i).Key;
			}

			else continue;
		}
	}

	// If there is a key mapping for steer, use that key mapping
	else
	{
		// Query for keys mapped to steer input action
		for (int i = 0; i < enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings().Num(); ++i)
		{
			if (enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].GetMappingName() == moveDownMapName)
				return enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::MoveUpKey() const
{
	if (!DefaultMappingContext || !MoveAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for steer, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(MoveAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == MoveAction &&
				DefaultMappingContext->GetMapping(i).GetMappingName() == moveUpMapName)
			{
				return DefaultMappingContext->GetMapping(i).Key;
			}

			else continue;
		}
	}

	// If there is a key mapping for steer, use that key mapping
	else
	{
		// Query for keys mapped to steer input action
		for (int i = 0; i < enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings().Num(); ++i)
		{
			if (enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].GetMappingName() == moveUpMapName)
				return enhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings()[i].Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::JumpKey() const
{
	if (!DefaultMappingContext || !JumpAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for jump, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(JumpAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == JumpAction)
				return DefaultMappingContext->GetMapping(i).Key;

			else continue;
		}
	}

	// If there is a key mapping for jump, use that key mapping
	else
	{
		// Query for keys mapped to jump input action
		for (const FKey& Key : enhancedInputSubsystem->QueryKeysMappedToAction(JumpAction))
		{
			// This includes both default and remapped keys
			return Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::InteractKey() const
{
	if (!DefaultMappingContext || !InteractAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for interact, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(InteractAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == InteractAction)
				return DefaultMappingContext->GetMapping(i).Key;

			else continue;
		}
	}

	// If there is a key mapping for interact, use that key mapping
	else
	{
		// Query for keys mapped to interact input action
		for (const FKey& Key : enhancedInputSubsystem->QueryKeysMappedToAction(InteractAction))
		{
			// This includes both default and remapped keys
			return Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::SprintKey() const
{
	if (!DefaultMappingContext || !SprintAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for sprint, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(SprintAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == SprintAction)
				return DefaultMappingContext->GetMapping(i).Key;

			else continue;
		}
	}

	// If there is a key mapping for sprint, use that key mapping
	else
	{
		// Query for keys mapped to sprint input action
		for (const FKey& Key : enhancedInputSubsystem->QueryKeysMappedToAction(SprintAction))
		{
			// This includes both default and remapped keys
			return Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::FireKey() const
{
	if (!DefaultMappingContext || !FireAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for fire, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(FireAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == FireAction)
				return DefaultMappingContext->GetMapping(i).Key;

			else continue;
		}
	}

	// If there is a key mapping for fire, use that key mapping
	else
	{
		// Query for keys mapped to fire input action
		for (const FKey& Key : enhancedInputSubsystem->QueryKeysMappedToAction(FireAction))
		{
			// This includes both default and remapped keys
			return Key;
		}
	}

	return FKey();
}

FKey ADefaultPlayerController::PauseKey() const
{
	if (!DefaultMappingContext || !PauseAction) return FKey();

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem) return FKey();

	// If there is no key mappings for pause, just use the key from the input mapping context
	if (enhancedInputSubsystem->QueryKeysMappedToAction(PauseAction).IsEmpty())
	{
		for (int i = 0; i < DefaultMappingContext->GetMappings().Num(); ++i)
		{
			if (DefaultMappingContext->GetMapping(i).Action == PauseAction)
				return DefaultMappingContext->GetMapping(i).Key;

			else continue;
		}
	}

	// If there is a key mapping for pause, use that key mapping
	else
	{
		// Query for keys mapped to pause input action
		for (const FKey& Key : enhancedInputSubsystem->QueryKeysMappedToAction(PauseAction))
		{
			// This includes both default and remapped keys
			return Key;
		}
	}

	return FKey();
}

bool ADefaultPlayerController::NewKeyConflictsWithActionMapping(const FKey& newKey_) const
{
	if (newKey_ == currentMoveLeftKey || newKey_ == currentMoveRightKey || newKey_ == currentMoveDownKey ||
		newKey_ == currentMoveUpKey || newKey_ == currentJumpKey || newKey_ == currentInteractKey ||
		newKey_ == currentSprintKey || newKey_ == currentFireKey || newKey_ == currentPauseKey)
	{
		return true;
	}

	return false;
}

void ADefaultPlayerController::UpdateMoveLeftInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !MoveAction) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = moveLeftMapName;
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentMoveLeftKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = moveLeftMapName;
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentMoveLeftKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateMoveRightInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !MoveAction) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = moveRightMapName;
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentMoveRightKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = moveRightMapName;
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentMoveRightKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateMoveDownInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !MoveAction) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = moveDownMapName;
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentMoveDownKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = moveDownMapName;
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentMoveDownKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateMoveUpInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !MoveAction) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = moveUpMapName;
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentMoveUpKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = moveUpMapName;
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentMoveUpKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateJumpInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !JumpAction ||
		!JumpAction->GetPlayerMappableKeySettings()) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = JumpAction->GetPlayerMappableKeySettings()->GetMappingName();
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentJumpKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = JumpAction->GetPlayerMappableKeySettings()->GetMappingName();
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentJumpKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateInteractInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !InteractAction ||
		!InteractAction->GetPlayerMappableKeySettings()) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = InteractAction->GetPlayerMappableKeySettings()->GetMappingName();
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentInteractKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = InteractAction->GetPlayerMappableKeySettings()->GetMappingName();
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentInteractKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateSprintInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !SprintAction ||
		!SprintAction->GetPlayerMappableKeySettings()) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = SprintAction->GetPlayerMappableKeySettings()->GetMappingName();
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentSprintKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = SprintAction->GetPlayerMappableKeySettings()->GetMappingName();
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentSprintKey = newKey_;
	}
}

void ADefaultPlayerController::UpdateFireInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !FireAction ||
		!FireAction->GetPlayerMappableKeySettings()) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = FireAction->GetPlayerMappableKeySettings()->GetMappingName();
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentFireKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = FireAction->GetPlayerMappableKeySettings()->GetMappingName();
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentFireKey = newKey_;
	}
}

void ADefaultPlayerController::UpdatePauseInput(const FKey& newKey_)
{
	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!enhancedInputSubsystem || !DefaultMappingContext || !PauseAction ||
		!PauseAction->GetPlayerMappableKeySettings()) return;

	if (UEnhancedInputUserSettings* Settings = enhancedInputSubsystem->GetUserSettings())
	{
		FMapPlayerKeyArgs previousArgs = {};
		previousArgs.MappingName = PauseAction->GetPlayerMappableKeySettings()->GetMappingName();
		previousArgs.Slot = EPlayerMappableKeySlot::First;
		previousArgs.NewKey = currentPauseKey;

		FMapPlayerKeyArgs Args = {};
		Args.MappingName = PauseAction->GetPlayerMappableKeySettings()->GetMappingName();
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = newKey_;

		FGameplayTagContainer previousFailureReason, FailureReason;
		Settings->UnMapPlayerKey(previousArgs, previousFailureReason);
		Settings->MapPlayerKey(Args, FailureReason);

		currentPauseKey = newKey_;
	}
}