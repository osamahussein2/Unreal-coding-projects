// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UImage;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHealthBar(float currentHealth, float maxHealth);
	void SetArmorBar(float currentArmor_, float maxArmor_);
	void SetUpdateBar(float currentValue_, float maxValue_);

	void SetHealthHUD(float& value_);

	// Current Held Weapon UI elements functions
	UTextBlock* GetWeaponAmmoCounterText() const { return weaponAmmoCounterText; }
	UImage* GetWeaponImage() const { return weaponImage; }

	void SetWeaponAmmoText();
	void SetWeaponAmmoText(int clipsInMagazine_, int totalAmmo_);

	void CollapseWeaponOverlay();
	void SetWeaponImage(UTexture2D* weaponTexture_);

	void InitializeRadialUpdateMaterial();

	//void SpawnRadialArmorMaterial();
	//void DestroyRadialArmor();

	//void DecreaseHealth();

private:
	// Private initialize functions
	void InitializeRadialHealthMaterial();
	void InitializeRadialArmorMaterial();

	// Private update functions
	void TriggerHealthFlash(float DeltaTime);
	void DisableHealthFlash();

	// Private getters for HUD
	bool HasLowHealth(float& setValue_) const;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* WeaponOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackOverlayImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* RadialHealthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* RadialArmorBar = nullptr;

	UPROPERTY(EditAnywhere)
	UProgressBar* RadialUpdatedBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* minimapImage = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI Materials")
	UMaterialInterface* radialHealthMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI Materials")
	UMaterialInterface* radialArmorMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI Materials")
	UMaterialInterface* radialUpdateMaterial = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* materialHealthInst = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* materialArmorInst = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* materialUpdateInst = nullptr;

	// Health HUD variables
	UPROPERTY(EditDefaultsOnly, Category = "Health HUD Colors")
	FSlateColor radialBarBackgroundColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f));

	UPROPERTY(EditDefaultsOnly, Category = "Health HUD Colors")
	FSlateColor radialLowHealthFillColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));

	UPROPERTY(EditDefaultsOnly, Category = "Health HUD Colors")
	FSlateColor radialHealthFillColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));

	UPROPERTY(EditDefaultsOnly, Category = "Health HUD Colors")
	FSlateColor radialArmorFillColor = FSlateColor(FLinearColor(0.0f, 1.0f, 1.0f));

	UPROPERTY(EditDefaultsOnly, Category = "Health HUD Colors")
	FSlateColor radialUpdatedFillColor = FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f));

	float healthHUD_value = 100.0f;

	UPROPERTY()
	float healthFlashTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health Flash Time")
	float flashHealthAt = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health Flash Time")
	float maxHealthHiddenTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Health Flash Time")
	float maxHealthVisibleTime = 1.0f;

	UPROPERTY()
	bool isHealthFlashing = false;

	UPROPERTY()
	bool reachedLowHealth = false;

	// Current Held Weapon UI elements
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* weaponAmmoCounterText = nullptr;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* weaponImage = nullptr;
};
