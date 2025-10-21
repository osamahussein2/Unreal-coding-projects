// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OffscreenRenderWidgetComponent.h"
#include "Engine/TextureRenderTarget2D.h"

UOffscreenRenderWidgetComponent::UOffscreenRenderWidgetComponent()
{
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::World);
	SetTickWhenOffscreen(true);
}

void UOffscreenRenderWidgetComponent::SetCustomRenderTarget(UTextureRenderTarget2D* InRenderTarget)
{
	if (!InRenderTarget) return;

	ExternalRenderTarget = InRenderTarget;

	bManuallyRedraw = true;
	bRedrawRequested = true;
	//UpdateRenderTarget(ExternalRenderTarget);
}

void UOffscreenRenderWidgetComponent::InitWidget()
{
	Super::InitWidget();
	if (ExternalRenderTarget)
	{
		//UpdateRenderTarget(ExternalRenderTarget);
	}
}
