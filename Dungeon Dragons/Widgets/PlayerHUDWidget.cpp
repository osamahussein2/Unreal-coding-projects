// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PlayerHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HowToUnlockDoorWidget.h"
#include "Widgets/CollapsibleInventoryWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateDieImage();

	if (inventoryWidget)
	{
		if (UCanvasPanelSlot* inventoryWidgetSlot = Cast<UCanvasPanelSlot>(inventoryWidget->Slot))
		{
			initialInventoryPos = inventoryWidgetSlot->GetPosition().Y;
		}
	}

	if (isInventoryPoppingUp) isInventoryPoppingUp = false;
}

void UPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (unlockDoorWidget)
	{
		unlockDoorWidget->RemoveFromParent();
		unlockDoorWidget = nullptr;
	}
	
	if (inventoryWidget)
	{
		if (isInventoryPoppingUp) isInventoryPoppingUp = false;

		inventoryWidget->RemoveFromParent();
		inventoryWidget = nullptr;
	}

	if (MinimapImage)
	{
		MinimapImage->RemoveFromParent();
		MinimapImage = nullptr;
	}

	if (CharacterPortrait)
	{
		CharacterPortrait->RemoveFromParent();
		CharacterPortrait = nullptr;
	}

	if (HealthBar)
	{
		HealthBar->RemoveFromParent();
		HealthBar = nullptr;
	}

	if (DexterityBar)
	{
		DexterityBar->RemoveFromParent();
		DexterityBar = nullptr;
	}

	if (StrengthBar)
	{
		StrengthBar->RemoveFromParent();
		StrengthBar = nullptr;
	}

	if (IntelligenceBar)
	{
		IntelligenceBar->RemoveFromParent();
		IntelligenceBar = nullptr;
	}

	if (HealthText)
	{
		HealthText->RemoveFromParent();
		HealthText = nullptr;
	}

	if (DexterityText)
	{
		DexterityText->RemoveFromParent();
		DexterityText = nullptr;
	}

	if (StrengthText)
	{
		StrengthText->RemoveFromParent();
		StrengthText = nullptr;
	}

	if (IntelligenceText)
	{
		IntelligenceText->RemoveFromParent();
		IntelligenceText = nullptr;
	}

	if (HealthPercentageText)
	{
		HealthPercentageText->RemoveFromParent();
		HealthPercentageText = nullptr;
	}

	if (DexterityPercentageText)
	{
		DexterityPercentageText->RemoveFromParent();
		DexterityPercentageText = nullptr;
	}

	if (StrengthPercentageText)
	{
		StrengthPercentageText->RemoveFromParent();
		StrengthPercentageText = nullptr;
	}

	if (IntelligencePercentageText)
	{
		IntelligencePercentageText->RemoveFromParent();
		IntelligencePercentageText = nullptr;
	}

	if (BlackOverlayImage)
	{
		BlackOverlayImage->RemoveFromParent();
		BlackOverlayImage = nullptr;
	}

	if (StatsOverlay)
	{
		StatsOverlay->RemoveFromParent();
		StatsOverlay = nullptr;
	}

	if (PlayerHUDCanvas)
	{
		PlayerHUDCanvas->RemoveFromParent();
		PlayerHUDCanvas = nullptr;
	}
}

void UPlayerHUDWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(PressToDestroyUnlockDoorWidget))
	{
		Super::NativeTick(Geometry, DeltaTime);

		DestroyUnlockDoorWidget();
	}
}

void UPlayerHUDWidget::SetHealthBar(float healthPercentage_)
{
	if (HealthBar) HealthBar->SetPercent(healthPercentage_);

	if (HealthPercentageText)
	{
		HealthPercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(healthPercentage_ * 100))));
	}
}

void UPlayerHUDWidget::SetPlayerStats(float dexterity_, float strength_, float intelligence_)
{
	if (DexterityBar) DexterityBar->SetPercent(dexterity_ / 100.0f);
	if (StrengthBar) StrengthBar->SetPercent(strength_ / 100.0f);
	if (IntelligenceBar) IntelligenceBar->SetPercent(intelligence_ / 100.0f);

	if (DexterityPercentageText)
	{
		DexterityPercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(dexterity_))));
	}

	if (StrengthPercentageText)
	{
		StrengthPercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(strength_))));
	}

	if (IntelligencePercentageText)
	{
		IntelligencePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(intelligence_))));
	}
}

void UPlayerHUDWidget::UpdateDieImage()
{
	if (DiceRoll)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			switch (player->GetDiceRollNumber())
			{
			case 1:
				if (dieTextures.Num() > 0 && dieTextures[0]) DiceRoll->SetBrushFromTexture(dieTextures[0]);
				break;

			case 2:
				if (dieTextures.Num() > 1 && dieTextures[1]) DiceRoll->SetBrushFromTexture(dieTextures[1]);
				break;

			case 3:
				if (dieTextures.Num() > 2 && dieTextures[2]) DiceRoll->SetBrushFromTexture(dieTextures[2]);
				break;

			case 4:
				if (dieTextures.Num() > 3 && dieTextures[3]) DiceRoll->SetBrushFromTexture(dieTextures[3]);
				break;

			case 5:
				if (dieTextures.Num() > 4 && dieTextures[4]) DiceRoll->SetBrushFromTexture(dieTextures[4]);
				break;

			case 6:
				if (dieTextures.Num() > 5 && dieTextures[5]) DiceRoll->SetBrushFromTexture(dieTextures[5]);
				break;

			default:
				DiceRoll->SetBrushFromTexture(nullptr);
				break;
			}
		}
	}
}

void UPlayerHUDWidget::SpawnUnlockDoorWidget()
{
	if (unlockDoorWidgetClass && !unlockDoorWidget)
	{
		unlockDoorWidget = WidgetTree->ConstructWidget<UHowToUnlockDoorWidget>(unlockDoorWidgetClass);
		if (!unlockDoorWidget) return;

		unlockDoorWidget->SetInstructionText(PressToDestroyUnlockDoorWidget);

		PlayerHUDCanvas->AddChildToCanvas(unlockDoorWidget);

		if (UCanvasPanelSlot* unlockDoorWidgetSlot = Cast<UCanvasPanelSlot>(unlockDoorWidget->Slot))
		{
			unlockDoorWidgetSlot->SetPosition(unlockDoorWidgetPosition);
			unlockDoorWidgetSlot->SetSize(unlockDoorWidgetSize);
			unlockDoorWidgetSlot->SetAlignment(unlockDoorWidgetAlignment);
			unlockDoorWidgetSlot->SetAutoSize(unlockDoorWidgetAutoSize);
		}
	}
}

void UPlayerHUDWidget::DestroyUnlockDoorWidget()
{
	if (unlockDoorWidget)
	{
		unlockDoorWidget->RemoveFromParent();
		unlockDoorWidget = nullptr;
	}
}

/*void UPlayerHUDWidget::SpawnInventoryWidget()
{
	if (inventoryWidgetClass && !inventoryWidget)
	{
		inventoryWidget = WidgetTree->ConstructWidget<UInventoryWidget>(inventoryWidgetClass);
		if (!inventoryWidget) return;

		PlayerHUDCanvas->AddChildToCanvas(inventoryWidget);

#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawned inventory"));
#endif

		if (UCanvasPanelSlot* inventoryWidgetSlot = Cast<UCanvasPanelSlot>(inventoryWidget->Slot))
		{
			inventoryWidgetSlot->SetPosition(inventoryWidgetPosition);
			inventoryWidgetSlot->SetSize(inventoryWidgetSize);
			inventoryWidgetSlot->SetAlignment(inventoryWidgetAlignment);
			inventoryWidgetSlot->SetAutoSize(inventoryWidgetAutoSize);
		}
	}
}*/

void UPlayerHUDWidget::PopInventoryUp()
{
	if (!isInventoryPoppingUp)
	{
		GetWorld()->GetTimerManager().SetTimer(showInventoryHandle, this, &UPlayerHUDWidget::MoveInventoryOnScreen,
			GetWorld()->GetDeltaSeconds(), true);

		isInventoryPoppingUp = true;
	}

	if (inventoryWidget) inventoryWidget->UpdateInventorySlot();
}

void UPlayerHUDWidget::MoveInventoryOnScreen()
{
	if (inventoryWidget)
	{
		if (UCanvasPanelSlot* inventoryWidgetSlot = Cast<UCanvasPanelSlot>(inventoryWidget->Slot))
		{
			inventoryPos += GetWorld()->GetDeltaSeconds() * -inventoryMoveSpeed;
			const FVector2D& moveInventory = inventoryWidgetSlot->GetPosition() + FVector2D(0.0f, inventoryPos);

			inventoryWidgetSlot->SetPosition(moveInventory);

#ifdef UE_EDITOR
			GEngine->AddOnScreenDebugMessage(300, 2.0f, FColor::Green, FString::FromInt(inventoryWidgetSlot->GetPosition().Y));
#endif

			if (inventoryWidgetSlot->GetPosition().Y < stoppingPositionY)
			{
				inventoryPos = 0.0f;

				GetWorld()->GetTimerManager().ClearTimer(showInventoryHandle);

				FTimerHandle startHidingInventoryHandle;
				GetWorld()->GetTimerManager().SetTimer(startHidingInventoryHandle, this, &UPlayerHUDWidget::StartHidingInventory,
					timeToHideInventory, false);
			}
		}
	}
}

void UPlayerHUDWidget::StartHidingInventory()
{
	GetWorld()->GetTimerManager().SetTimer(hideInventoryHandle, this, &UPlayerHUDWidget::MoveInventoryOffScreen,
		GetWorld()->GetDeltaSeconds(), true);
}

void UPlayerHUDWidget::MoveInventoryOffScreen()
{
	if (inventoryWidget)
	{
		if (UCanvasPanelSlot* inventoryWidgetSlot = Cast<UCanvasPanelSlot>(inventoryWidget->Slot))
		{
			inventoryPos += GetWorld()->GetDeltaSeconds() * inventoryMoveSpeed;
			const FVector2D& moveInventory = inventoryWidgetSlot->GetPosition() + FVector2D(0.0f, inventoryPos);

			inventoryWidgetSlot->SetPosition(moveInventory);

#ifdef UE_EDITOR
			GEngine->AddOnScreenDebugMessage(300, 2.0f, FColor::Green, FString::FromInt(inventoryWidgetSlot->GetPosition().Y));
#endif

			if (inventoryWidgetSlot->GetPosition().Y > initialInventoryPos)
			{
				inventoryPos = 0.0f;

				GetWorld()->GetTimerManager().ClearTimer(hideInventoryHandle);
				isInventoryPoppingUp = false;
			}
		}
	}
}
