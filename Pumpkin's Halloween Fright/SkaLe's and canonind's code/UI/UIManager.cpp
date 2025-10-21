// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"
#include "UI/BaseUIWidget.h"
#include "UI/UIRenderRoot.h"
#include "Characters/HJPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/CanvasPanel.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

AUIManager::AUIManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRenderUIToTarget && UIRenderTarget && UIRenderComponent)
	{
		UpdateRenderTarget();

	}
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	if (bRenderUIToTarget)
	{
		SpawnWidgetComponent();
	}

	if (bRenderUIToTarget && UIRenderTarget && UIRootWidgetClass)
	{
		UIRenderComponent->SetWidgetClass(UIRootWidgetClass);
		UIRenderComponent->SetDrawSize(FIntPoint(UIRenderTarget->SizeX, UIRenderTarget->SizeY));
		UIRenderComponent->InitWidget();

		UIRootWidgetInstance = Cast<UUIRenderRoot>(UIRenderComponent->GetWidget());

		if (!UIRootWidgetInstance)
		{
			UE_LOG(LogTemp, Error, TEXT("UIManager: Failed to create UIRootWidgetInstance!"));
		}
		if (CopyMaterial)
		{
			CopyMaterialInstance = UMaterialInstanceDynamic::Create(CopyMaterial, this);
		}

	}
	bIsInitialized = true;
	OnUIManagerReady.Broadcast(this);



	// 	if (bAlwaysShowMouseCursor)
	// 	{
	// 		APlayerController* PC = GetOwningPlayerController();
	// 		if (PC)
	// 		{
	// 			FInputModeGameAndUI InputMode;
	// 			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 			InputMode.SetHideCursorDuringCapture(false);
	// 			PC->SetInputMode(InputMode);
	// 			PC->bShowMouseCursor = true;
	// 			PC->bEnableClickEvents = true;
	// 			PC->bEnableMouseOverEvents = true;
	// 		}
	// 	}

}

void AUIManager::UpdateRenderTarget()
{
	if (!InternalRenderTarget)
	{
		// Try to get the internal render target using reflection
		FProperty* RenderTargetProperty = UIRenderComponent->GetClass()->FindPropertyByName(TEXT("RenderTarget"));
		if (RenderTargetProperty)
		{
			FObjectProperty* ObjectProperty = CastField<FObjectProperty>(RenderTargetProperty);
			if (ObjectProperty)
			{
				InternalRenderTarget = Cast<UTextureRenderTarget2D>(
					ObjectProperty->GetObjectPropertyValue_InContainer(UIRenderComponent)
				);
			}
		}
		if (!InternalRenderTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIManager: Failed to find internal render target!"));
			return;
		}
	}

	if (InternalRenderTarget && UIRenderTarget)
	{
		if (CopyMaterialInstance)
		{
			UKismetRenderingLibrary::ClearRenderTarget2D(this, UIRenderTarget);
			CopyMaterialInstance->SetTextureParameterValue(TEXT("SourceTexture"), InternalRenderTarget);
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(
				GetWorld(),
				UIRenderTarget,
				CopyMaterialInstance
			);
		}
	}
}

void AUIManager::SpawnWidgetComponent()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// We spawn new actor because we cannot attach widget component to HUD class. It won't be visible that way

	AActor* WidgetActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WidgetActor)
	{
		USceneComponent* NewRootComponent = NewObject<USceneComponent>(WidgetActor, USceneComponent::StaticClass(), TEXT("SceneRoot"));

		if (NewRootComponent)
		{
			WidgetActor->SetRootComponent(NewRootComponent);
			NewRootComponent->RegisterComponent();
		}

		WidgetActor->SetActorLocation(FVector(0, 0, 100000)); // Spawn really far to be outside of the visible scene

		UWidgetComponent* NewWidgetComp = NewObject<UWidgetComponent>(WidgetActor, UWidgetComponent::StaticClass(), TEXT("UIRenderComponent"));
		if (NewWidgetComp)
		{
			NewWidgetComp->RegisterComponent();
			NewWidgetComp->AttachToComponent(WidgetActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

			NewWidgetComp->SetWidgetSpace(EWidgetSpace::World);
			NewWidgetComp->SetVisibility(true);
			NewWidgetComp->SetTickWhenOffscreen(true);
			NewWidgetComp->SetBackgroundColor(FLinearColor::Transparent);
			NewWidgetComp->SetTickMode(ETickMode::Enabled);
			NewWidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewWidgetComp->PrimaryComponentTick.bTickEvenWhenPaused = true;

			UIRenderComponent = NewWidgetComp;
		}
	}
}


UBaseUIWidget* AUIManager::CreateWidgetInstance(TSubclassOf<UBaseUIWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return nullptr;
	}

	UBaseUIWidget* Widget = CreateWidget<UBaseUIWidget>(PC, WidgetClass);
	if (Widget)
	{
		Widget->SetOwningManager(this);

		if (bRenderUIToTarget)
		{
			if (!UIRootWidgetInstance && UIRenderComponent)
			{
				UIRootWidgetInstance = Cast<UUIRenderRoot>(UIRenderComponent->GetUserWidgetObject());
			}
			if (UIRootWidgetInstance)
			{
				UIRootWidgetInstance->AddWidgetToRoot(Widget);
			}
			else
			{
				// Root widget not ready yet.Widget is created but will be parented later in BeginPlay
			}
		}

		// Cache widget to use it next time it's class is pushed
		WidgetCache.Add(WidgetClass, Widget);
	}
	return Widget;
}


void AUIManager::HideTopWidget()
{
	// Trigger hide for top widget

	// To prevent calling TriggerHide for the same widget more than one time
	if (!bAllowTopWidgetHide) return;
	if (WidgetStack.Num() == 0) return;

	UBaseUIWidget* TopWidget = WidgetStack.Top();
	TopWidget->TriggerHide();
}


void AUIManager::OnTriggerHideFinished()
{
	// Finished animating hide (or skipped)
	PopWidget();
	bAllowTopWidgetHide = true;
}



UBaseUIWidget* AUIManager::PushWidget(TSubclassOf<UBaseUIWidget> WidgetClass)
{
	if (!WidgetClass) return nullptr;

	UBaseUIWidget* NewWidget = nullptr;
	TObjectPtr<UBaseUIWidget>* CachedWidgetPtr = nullptr;
	if (WidgetCache.Num() > 0)
	{
		CachedWidgetPtr = WidgetCache.Find(WidgetClass);
	}
	if (CachedWidgetPtr && *CachedWidgetPtr)
	{
		NewWidget = CachedWidgetPtr->Get();
	}
	else
	{
		NewWidget = CreateWidgetInstance(WidgetClass);
	}

	if (!NewWidget)
	{
		return nullptr;
	}
	return PushWidgetInstance(NewWidget);
}

UBaseUIWidget* AUIManager::PushWidgetInstance(UBaseUIWidget* Widget)
{
	UBaseUIWidget* CurrentTop = GetTopWidget();
	if (CurrentTop)
	{
		if (Widget->bShouldHideLowerWidget)
		{
			CurrentTop->SetVisibility(ESlateVisibility::Collapsed);
		}
		CurrentTop->OnWidgetHidden();
	}

	WidgetStack.Add(Widget);
	if (!bRenderUIToTarget)
	{
		Widget->AddToViewport(200);
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TrySetRootWidgetInstanceIfInvalid();
		if (UIRootWidgetInstance)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (Widget->bPauseGameWhenShown)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	if (Widget->bDisablePlayerInput)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC && PC->GetPawn())
		{
			PC->GetPawn()->DisableInput(PC);
		}
	}

	Widget->OnWidgetShown();

	SetInputModeForWidget(Widget);
	return Widget;
}

void AUIManager::TrySetRootWidgetInstanceIfInvalid()
{
	if (!UIRootWidgetInstance && UIRenderComponent)
	{
		UIRootWidgetInstance = Cast<UUIRenderRoot>(UIRenderComponent->GetUserWidgetObject());
	}
}

void AUIManager::PopWidget()
{
	if (WidgetStack.Num() == 0)
	{
		return;
	}

	UBaseUIWidget* TopWidget = WidgetStack.Pop();
	if (TopWidget)
	{
		TopWidget->OnWidgetHidden();
		if (!bRenderUIToTarget)
		{
			TopWidget->RemoveFromParent();
		}
		else
		{
			if (!UIRootWidgetInstance && UIRenderComponent)
			{
				UIRootWidgetInstance = Cast<UUIRenderRoot>(UIRenderComponent->GetUserWidgetObject());
			}
			if (UIRootWidgetInstance)
			{
				//UIRootWidgetInstance->RemoveWidgetFromRoot(TopWidget);
				TopWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		if (TopWidget->bDisablePlayerInput)
		{
			APlayerController* PC = GetOwningPlayerController();
			if (PC && PC->GetPawn())
			{
				PC->GetPawn()->EnableInput(PC);
			}
		}
	}

	UBaseUIWidget* NewTop = GetTopWidget();
	if (NewTop)
	{
		if (!NewTop->bPauseGameWhenShown)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		NewTop->SetVisibility(ESlateVisibility::Visible);
		NewTop->OnWidgetShown();
		SetInputModeForWidget(NewTop);
	}
	else
	{
		RestoreGameInputMode();
	}
}

void AUIManager::ClearWidgetStack()
{
	while (WidgetStack.Num() > 0)
	{
		PopWidget();
	}
}

UBaseUIWidget* AUIManager::GetTopWidget() const
{
	return WidgetStack.Num() > 0 ? WidgetStack.Last() : nullptr;
}

bool AUIManager::DoesTopWidgetAllowPause() const
{
	UBaseUIWidget* TopWidget = GetTopWidget();
	if (TopWidget)
	{
		return TopWidget->bAllowsPause;
	}
	return true;
}

bool AUIManager::ShouldHideTopWidgetOnPauseToggleInput() const
{
	UBaseUIWidget* TopWidget = GetTopWidget();
	if (TopWidget)
	{
		return TopWidget->bHideOnPauseToggle;
	}
	return false;
}

FVector2D AUIManager::GetRenderTargetSize() const
{
	return UIRenderTarget ? FVector2D(UIRenderTarget->SizeX, UIRenderTarget->SizeY) : FVector2D::ZeroVector;
}

void AUIManager::SetInputModeForWidget(UBaseUIWidget* Widget)
{
	if (!Widget) return;
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	FInputModeGameAndUI InputMode;
	if (!bRenderUIToTarget)
	{
		InputMode.SetWidgetToFocus(Widget->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	PC->SetInputMode(InputMode);

	PC->bShowMouseCursor = Widget->bShowMouseCursor;
	PC->bEnableClickEvents = Widget->bShowMouseCursor;
	PC->bEnableMouseOverEvents = Widget->bShowMouseCursor;
}

void AUIManager::RestoreGameInputMode()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (bAlwaysShowMouseCursor)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
		PC->bEnableMouseOverEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
	else
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
	}
}
