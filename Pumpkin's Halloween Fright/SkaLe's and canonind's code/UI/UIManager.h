// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIManager.generated.h"

class AUIManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUIManagerReadySignature, AUIManager*, UIManager);

class UUserWidget;
class UBaseUIWidget;
class UWidgetComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UMaterialInstanceDynamic;
class UUIRenderRoot;

/**
 * Central UI Manager. Handles UI creation, stacking, and transitions.
 */
UCLASS()
class GDTV_HALLOWEEN2025_API AUIManager : public AHUD
{
	GENERATED_BODY()
	
public:
	AUIManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnTriggerHideFinished();


	/* Generic widget mangement */
	UFUNCTION(BlueprintCallable, Category = "UI")
	UBaseUIWidget* PushWidget(TSubclassOf<UBaseUIWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UBaseUIWidget* PushWidgetInstance(UBaseUIWidget* Widget);

	void TrySetRootWidgetInstanceIfInvalid();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PopWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideTopWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearWidgetStack();

	UFUNCTION(BlueprintPure, Category = "UI")
	UBaseUIWidget* GetTopWidget() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool DoesTopWidgetAllowPause() const; 

	UFUNCTION(BlueprintPure, Category = "UI")
	bool ShouldHideTopWidgetOnPauseToggleInput() const;

	/* Render to RT Helpers */
	UFUNCTION(BlueprintPure, Category = "UI")
	UWidgetComponent* GetUIRenderComponent() const { return UIRenderComponent;}
	UFUNCTION(BlueprintPure, Category = "UI")
	UTextureRenderTarget2D* GetUIRenderTarget() const {return UIRenderTarget;}
	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsRenderingToTarget() const {return bRenderUIToTarget;}
	UFUNCTION(BlueprintPure, Category = "UI")
	FVector2D GetRenderTargetSize() const;

protected:
	/* Helper functions */
	UBaseUIWidget* CreateWidgetInstance(TSubclassOf<UBaseUIWidget> WidgetClass);
	void SetInputModeForWidget(UBaseUIWidget* Widget);
	void RestoreGameInputMode();
	void UpdateRenderTarget();
	void SpawnWidgetComponent();



public:
	FUIManagerReadySignature OnUIManagerReady;
	bool bIsInitialized = false;
protected:

	/* Widget stack for managing layered UI */
	UPROPERTY()
	TArray<TObjectPtr<UBaseUIWidget>> WidgetStack;

	UPROPERTY()
	TMap<TSubclassOf<UBaseUIWidget>, TObjectPtr<UBaseUIWidget>> WidgetCache;

	// ========================================
	// ------ UI Rrender-target support -------
	// ========================================
	
	/* If true, AUIManager will render into UIRenderTarget. When false, widgets are added to the viewport */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|RenderTarget")
	bool bRenderUIToTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|RenderTarget")
	UTextureRenderTarget2D* UIRenderTarget = nullptr;

	/* WidgetComponent that will render the Root UI widget into the UIRenderTarget */
	UPROPERTY(VisibleAnywhere, Category = "UI|RenderTarget")
	TObjectPtr<UWidgetComponent> UIRenderComponent = nullptr;

	/* The class of the root UI widget that will contain all other widgets when rendering to RT*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|RenderTarget")
	TSubclassOf<UUIRenderRoot> UIRootWidgetClass;

	UPROPERTY()
	UUIRenderRoot* UIRootWidgetInstance = nullptr;

	/* Material used to copy from widget component's internal RT to our custom RT */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|RenderTarget")
	UMaterialInterface* CopyMaterial = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* CopyMaterialInstance = nullptr;

	/* Cached internal render target from WidgetComponent */
	UPROPERTY()
	UTextureRenderTarget2D* InternalRenderTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bAlwaysShowMouseCursor = false;

	bool bAllowTopWidgetHide = true;
};
