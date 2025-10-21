// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIRenderRoot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UCanvasPanelSlot* UUIRenderRoot::AddWidgetToRoot(UUserWidget* WidgetToAdd, int32 ZOrder /*= 0*/)
{
	if (!WidgetToAdd || !RootCanvas)
	{
		return nullptr;
	}

	if (WidgetToAdd->GetParent())
	{
		WidgetToAdd->RemoveFromParent();
	}

	UCanvasPanelSlot* NewSlot = RootCanvas->AddChildToCanvas(WidgetToAdd);
	if (NewSlot)
	{
		NewSlot->SetZOrder(ZOrder);
		NewSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		NewSlot->SetOffsets(FMargin(0.f));
	}
	return NewSlot;
}

void UUIRenderRoot::RemoveWidgetFromRoot(UUserWidget* WidgetToRemove)
{
	if (!WidgetToRemove || !RootCanvas)
	{
		return;
	}
	WidgetToRemove->RemoveFromParent();
}
