// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeRadialHealthMaterial();
	InitializeRadialArmorMaterial();
}

void UPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (minimapImage)
	{
		minimapImage->RemoveFromParent();
		minimapImage = nullptr;
	}

	if (RadialUpdatedBar)
	{
		RadialUpdatedBar->RemoveFromParent();
		RadialUpdatedBar = nullptr;
	}

	if (RadialArmorBar)
	{
		RadialArmorBar->RemoveFromParent();
		RadialArmorBar = nullptr;
	}

	if (RadialHealthBar)
	{
		RadialHealthBar->RemoveFromParent();
		RadialHealthBar = nullptr;
	}

	if (BlackOverlayImage)
	{
		BlackOverlayImage->RemoveFromParent();
		BlackOverlayImage = nullptr;
	}

	if (WeaponOverlay)
	{
		WeaponOverlay->RemoveFromParent();
		WeaponOverlay = nullptr;
	}

	if (PlayerHUDCanvas)
	{
		PlayerHUDCanvas->RemoveFromParent();
		PlayerHUDCanvas = nullptr;
	}
}

void UPlayerHUDWidget::SetHealthBar(float currentHealth, float maxHealth)
{
	if (RadialHealthBar && materialHealthInst)
	{
		// Convert this health HUD value to a reference object for the time being to prevent copying variables
		static_cast<float&>(healthHUD_value) = currentHealth;

		// Modify the radial health bar through the material parameter not the progress bar set percent method
		materialHealthInst->SetScalarParameterValue(TEXT("HealthValue"), currentHealth / maxHealth);
	}
}

/*void UPlayerHUDWidget::UpdateHealthBar(float updatedHealth, float maxHealth)
{
	if (DynamicHealthBar)
	{
		DynamicHealthBar->SetPercent(updatedHealth / maxHealth);
	}
}*/

void UPlayerHUDWidget::SetArmorBar(float currentArmor_, float maxArmor_)
{
	if (RadialArmorBar && materialArmorInst)
	{
		// Modify the radial armor bar through the material parameter not the progress bar set percent method
		materialArmorInst->SetScalarParameterValue(TEXT("ArmorValue"), currentArmor_ / maxArmor_);

		float armorLocationUpdate = 1.5f + (1.0f - (healthHUD_value / 100.0f));
		armorLocationUpdate = FMath::Clamp(armorLocationUpdate, 1.5f, 2.5f);

		materialArmorInst->SetScalarParameterValue(TEXT("CurrentHealth"), armorLocationUpdate);
	}
}

void UPlayerHUDWidget::SetUpdateBar(float currentValue_, float maxValue_)
{
	if (RadialUpdatedBar && materialUpdateInst)
	{
		materialUpdateInst->SetScalarParameterValue(TEXT("UpdateValue"), currentValue_ / maxValue_);
	}
}

void UPlayerHUDWidget::SetHealthHUD(float& value_)
{
	healthHUD_value = value_;
}

void UPlayerHUDWidget::SetWeaponAmmoText()
{
	if (weaponAmmoCounterText)
	{
		weaponAmmoCounterText->SetText(FText::FromString(TEXT("")));
	}
}

void UPlayerHUDWidget::SetWeaponAmmoText(int clipsInMagazine_, int totalAmmo_)
{
	if (weaponAmmoCounterText)
	{
		weaponAmmoCounterText->SetText(FText::FromString(FString::FromInt(clipsInMagazine_) + TEXT("-") + FString::FromInt(totalAmmo_)));
	}
}

void UPlayerHUDWidget::CollapseWeaponOverlay()
{
	if (WeaponOverlay && WeaponOverlay->GetVisibility() != ESlateVisibility::Collapsed)
	{
		WeaponOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerHUDWidget::SetWeaponImage(UTexture2D* weaponTexture_)
{
	if (WeaponOverlay && WeaponOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		WeaponOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (weaponImage && weaponTexture_)
	{
		weaponImage->SetBrushFromTexture(weaponTexture_);
	}
}

void UPlayerHUDWidget::InitializeRadialHealthMaterial()
{
	// Create the material instance from the material interface only if it's valid
	if (radialHealthMaterial)
	{
		materialHealthInst = UMaterialInstanceDynamic::Create(radialHealthMaterial, this);
	}

	// If we don't have a radial progress bar or a valid material instance, skip running the code below
	if (!RadialHealthBar || !materialHealthInst) return;

	// Otherwise, check for the radial progress bar widget style and create a brush from it
	FProgressBarStyle progressBarStyle = RadialHealthBar->GetWidgetStyle();
	FSlateBrush backgroundBrush, fillBrush;

	// Set the resource object from the brush to the material instance that was created earlier
	backgroundBrush.SetResourceObject(radialHealthMaterial);
	backgroundBrush.TintColor = radialBarBackgroundColor;

	fillBrush.SetResourceObject(materialHealthInst);
	fillBrush.TintColor = radialHealthFillColor;

	progressBarStyle.SetBackgroundImage(backgroundBrush); // Set background image of the progress bar using the brush variable
	progressBarStyle.SetFillImage(fillBrush); // Set fill image of the progress bar using the brush variable

	RadialHealthBar->SetWidgetStyle(progressBarStyle); // Set the progress bar style to its set widget style

	radialHealthMaterial = nullptr; // Start releasing radial bar material from memory after its material is created
}

void UPlayerHUDWidget::InitializeRadialArmorMaterial()
{
	// Create the material instance from the material interface only if it's valid
	if (radialArmorMaterial)
	{
		materialArmorInst = UMaterialInstanceDynamic::Create(radialArmorMaterial, this);
	}

	// If we don't have a radial progress bar or a valid material instance, skip running the code below
	if (!RadialArmorBar || !materialArmorInst) return;

	// Otherwise, check for the radial progress bar widget style and create a brush from it
	FProgressBarStyle progressBarStyle = RadialArmorBar->GetWidgetStyle();
	FSlateBrush backgroundBrush, fillBrush;

	// Set the resource object from the brush to the material instance that was created earlier
	backgroundBrush.SetResourceObject(radialArmorMaterial);
	backgroundBrush.TintColor = radialBarBackgroundColor;

	fillBrush.SetResourceObject(materialArmorInst);
	fillBrush.TintColor = radialArmorFillColor;

	progressBarStyle.SetBackgroundImage(backgroundBrush); // Set background image of the progress bar using the brush variable
	progressBarStyle.SetFillImage(fillBrush); // Set fill image of the progress bar using the brush variable

	RadialArmorBar->SetWidgetStyle(progressBarStyle); // Set the progress bar style to its set widget style

	radialArmorMaterial = nullptr; // Start releasing radial bar material from memory after its material is created
}

//void UPlayerHUDWidget::SpawnRadialArmorMaterial()
//{
//	if (!RadialArmorBar)
//	{
//		// Instantiate a radial bar that updates overtime based on player's health and armor values
//		RadialArmorBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
//
//		PlayerHUDCanvas->AddChildToCanvas(RadialArmorBar);
//
//		// Use the already created radial bars to check if their canvas slots are valid and change their z order if needed
//		UCanvasPanelSlot* RadialHealthCanvasSlot = Cast<UCanvasPanelSlot>(RadialHealthBar->Slot);
//		UCanvasPanelSlot* RadialArmorCanvasSlot = Cast<UCanvasPanelSlot>(RadialArmorBar->Slot);
//
//		if (!RadialHealthCanvasSlot || !RadialArmorCanvasSlot) return;
//
//		// Place the radial updated bar where one of the radial canvas slots are
//		RadialArmorCanvasSlot->SetAnchors(RadialHealthCanvasSlot->GetAnchors());
//		RadialArmorCanvasSlot->SetPosition(RadialHealthCanvasSlot->GetPosition());
//		RadialArmorCanvasSlot->SetAlignment(RadialHealthCanvasSlot->GetAlignment());
//		RadialArmorCanvasSlot->SetSize(RadialHealthCanvasSlot->GetSize());
//		RadialArmorCanvasSlot->SetZOrder(0);
//
//		// Release any memory if possible
//		RadialHealthCanvasSlot = nullptr;
//		RadialArmorCanvasSlot = nullptr;
//
//		// Create the material instance from the material interface only if it's valid
//		if (radialArmorMaterial)
//		{
//			materialArmorInst = UMaterialInstanceDynamic::Create(radialArmorMaterial, this);
//		}
//
//		// If we don't have a radial progress bar or a valid material instance, skip running the code below
//		if (!RadialArmorBar || !materialArmorInst) return;
//
//		// Otherwise, check for the radial progress bar widget style and create a brush from it
//		FProgressBarStyle progressBarStyle = RadialArmorBar->GetWidgetStyle();
//		FSlateBrush backgroundBrush, fillBrush;
//
//		// Set the resource object from the brush to the material instance that was created earlier
//		backgroundBrush.SetResourceObject(radialArmorMaterial);
//		backgroundBrush.TintColor = radialBarBackgroundColor;
//
//		fillBrush.SetResourceObject(materialArmorInst);
//		fillBrush.TintColor = radialArmorFillColor;
//
//		progressBarStyle.SetBackgroundImage(backgroundBrush); // Set background image of the progress bar using the brush variable
//		progressBarStyle.SetFillImage(fillBrush); // Set fill image of the progress bar using the brush variable
//
//		RadialArmorBar->SetWidgetStyle(progressBarStyle); // Set the progress bar style to its set widget style
//
//		RadialArmorBar->SetPercent(1.0f);
//
//		radialArmorMaterial = nullptr; // Start releasing radial bar material from memory after its material is created
//	}
//}
//
//void UPlayerHUDWidget::DestroyRadialArmor()
//{
//	if (RadialArmorBar)
//	{
//		if (materialArmorInst) materialArmorInst = nullptr;
//
//		RadialArmorBar->RemoveFromParent();
//		RadialArmorBar = nullptr;
//	}
//}

void UPlayerHUDWidget::InitializeRadialUpdateMaterial()
{
	if (!RadialUpdatedBar)
	{
		// Instantiate a radial bar that updates overtime based on player's health and armor values
		RadialUpdatedBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());

		PlayerHUDCanvas->AddChildToCanvas(RadialUpdatedBar);

		// Use the already created radial bars to check if their canvas slots are valid and change their z order if needed
		UCanvasPanelSlot* RadialHealthCanvasSlot = Cast<UCanvasPanelSlot>(RadialHealthBar->Slot);
		UCanvasPanelSlot* RadialArmorCanvasSlot = Cast<UCanvasPanelSlot>(RadialArmorBar->Slot);

		// Create the actual radial updated canvas slot
		UCanvasPanelSlot* RadialUpdatedCanvasSlot = Cast<UCanvasPanelSlot>(RadialUpdatedBar->Slot);

		if (!RadialHealthCanvasSlot || !RadialArmorCanvasSlot || !RadialUpdatedCanvasSlot) return;

		// Place the radial updated bar where one of the radial canvas slots are
		RadialUpdatedCanvasSlot->SetAnchors(RadialHealthCanvasSlot->GetAnchors());
		RadialUpdatedCanvasSlot->SetPosition(RadialHealthCanvasSlot->GetPosition());
		RadialUpdatedCanvasSlot->SetAlignment(RadialHealthCanvasSlot->GetAlignment());
		RadialUpdatedCanvasSlot->SetSize(RadialHealthCanvasSlot->GetSize());
		RadialUpdatedCanvasSlot->SetZOrder(0);

		RadialHealthCanvasSlot->SetZOrder(1); // Should be more important than the radial updated bar
		RadialArmorCanvasSlot->SetZOrder(1); // Should be more important than the radial updated bar

		// Release any memory if possible
		RadialUpdatedCanvasSlot = nullptr;
		RadialHealthCanvasSlot = nullptr;
		RadialArmorCanvasSlot = nullptr;
	}

	if (radialUpdateMaterial && !materialUpdateInst)
	{
		materialUpdateInst = UMaterialInstanceDynamic::Create(radialUpdateMaterial, this);
		radialUpdateMaterial = nullptr;
	}

	// If we don't have a radial progress bar or a valid material instance, skip running the code below
	if (!RadialUpdatedBar || !materialUpdateInst) return;

	// Otherwise, check for the radial progress bar widget style and create a brush from it
	FProgressBarStyle UpdateBar = RadialUpdatedBar->GetWidgetStyle();
	FSlateBrush updateBrush;

	// Set the resource object from the brush to the material instance that was created earlier
	updateBrush.SetResourceObject(materialUpdateInst);
	UpdateBar.SetFillImage(updateBrush); // Set fill image of the progress bar using the brush variable
	UpdateBar.FillImage.TintColor = radialUpdatedFillColor;
	UpdateBar.BackgroundImage.TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	RadialUpdatedBar->SetWidgetStyle(UpdateBar); // Set the progress bar style to its set widget style
}

bool UPlayerHUDWidget::HasLowHealth(float& setValue_) const
{
	return RadialHealthBar && healthHUD_value > 0.0f && healthHUD_value <= setValue_;
}

void UPlayerHUDWidget::TriggerHealthFlash(float DeltaTime)
{
	healthFlashTimer += DeltaTime;

	if (!reachedLowHealth) reachedLowHealth = true; // Make sure reached low health is set to true

	// Start hiding Health HUD
	if (healthFlashTimer <= maxHealthHiddenTime && !isHealthFlashing)
	{
		FProgressBarStyle armorBar = RadialHealthBar->GetWidgetStyle();
		FProgressBarStyle updateBar = RadialUpdatedBar->GetWidgetStyle();

		FSlateColor color = FSlateColor(FLinearColor(radialLowHealthFillColor.GetSpecifiedColor().R,
			radialLowHealthFillColor.GetSpecifiedColor().G,
			radialLowHealthFillColor.GetSpecifiedColor().B, 0.0f));

		FSlateColor updateColor = FSlateColor(FLinearColor(radialUpdatedFillColor.GetSpecifiedColor().R,
			radialUpdatedFillColor.GetSpecifiedColor().G,
			radialUpdatedFillColor.GetSpecifiedColor().B, 0.0f));

		armorBar.FillImage.TintColor = color;
		updateBar.FillImage.TintColor = updateColor;

		RadialUpdatedBar->SetWidgetStyle(updateBar);
		RadialHealthBar->SetWidgetStyle(armorBar);

		isHealthFlashing = true;
	}

	// Start showing Health HUD
	else if (healthFlashTimer >= maxHealthHiddenTime && healthFlashTimer <= maxHealthVisibleTime && isHealthFlashing)
	{
		FProgressBarStyle armorBar = RadialHealthBar->GetWidgetStyle();

		FSlateColor color = FSlateColor(FLinearColor(radialLowHealthFillColor.GetSpecifiedColor().R,
			radialLowHealthFillColor.GetSpecifiedColor().G,
			radialLowHealthFillColor.GetSpecifiedColor().B, 1.0f));

		armorBar.FillImage.TintColor = color;

		RadialHealthBar->SetWidgetStyle(armorBar);

		isHealthFlashing = false;
	}

	// Restart health flash timer back to 0
	else if (healthFlashTimer > maxHealthVisibleTime) healthFlashTimer = 0.0f;
}

void UPlayerHUDWidget::DisableHealthFlash()
{
	// Make sure the radial health bar shows up and reset all health flash related variables back to initialized values
	if (reachedLowHealth)
	{
		FProgressBarStyle armorBar = RadialHealthBar->GetWidgetStyle();
		FProgressBarStyle updateBar = RadialUpdatedBar->GetWidgetStyle();

		FSlateColor color = FSlateColor(FLinearColor(radialHealthFillColor.GetSpecifiedColor().R,
			radialHealthFillColor.GetSpecifiedColor().G,
			radialHealthFillColor.GetSpecifiedColor().B, 1.0f));

		FSlateColor updateColor = FSlateColor(FLinearColor(radialUpdatedFillColor.GetSpecifiedColor().R,
			radialUpdatedFillColor.GetSpecifiedColor().G,
			radialUpdatedFillColor.GetSpecifiedColor().B, 1.0f));

		armorBar.FillImage.TintColor = color;
		updateBar.FillImage.TintColor = updateColor;

		RadialUpdatedBar->SetWidgetStyle(updateBar);
		RadialHealthBar->SetWidgetStyle(armorBar); // Set the progress bar style to its set widget style

		if (healthFlashTimer != 0.0f) healthFlashTimer = 0.0f;
		if (isHealthFlashing) isHealthFlashing = false;
		reachedLowHealth = false;
	}
}