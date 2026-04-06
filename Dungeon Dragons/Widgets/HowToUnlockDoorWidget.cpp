// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/HowToUnlockDoorWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"
#include "Actor/Door.h"
#include "EngineUtils.h"

UHowToUnlockDoorWidget::UHowToUnlockDoorWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UHowToUnlockDoorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HowToUnlockDoorOverlay && !HowToUnlockDoorOverlay->IsVisible()) HowToUnlockDoorOverlay->SetVisibility(ESlateVisibility::Visible);
	if (BlackImage && !BlackImage->IsVisible()) BlackImage->SetVisibility(ESlateVisibility::Visible);
	if (HowToUnlockDoorText && !HowToUnlockDoorText->IsVisible()) HowToUnlockDoorText->SetVisibility(ESlateVisibility::Visible);
	if (InstructionText && !InstructionText) InstructionText->SetVisibility(ESlateVisibility::Visible);

	if (HowToUnlockDoorText)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			for (TActorIterator<ADoor> doorIt(GetWorld()); doorIt; ++doorIt)
			{
				if (!doorIt->GetDoorOpening() && player->IsDoorNearby(*doorIt))
				{
					switch (doorIt->GetDoorUnlocked())
					{
					case EDoorUnlocked::EDexterity:

						HowToUnlockDoorText->SetText(FText::FromString(TEXT("You need ") +
							FString::FromInt((*doorIt)->GetUnlockedAfterReachingSkill()) +
							TEXT(" dexterity to unlock this door!")));

						break;

					case EDoorUnlocked::EStrength:

						HowToUnlockDoorText->SetText(FText::FromString(TEXT("You need ") +
							FString::FromInt((*doorIt)->GetUnlockedAfterReachingSkill()) +
							TEXT(" strength to unlock this door!")));

						break;

					case EDoorUnlocked::EIntelligence:

						HowToUnlockDoorText->SetText(FText::FromString(TEXT("You need ") +
							FString::FromInt((*doorIt)->GetUnlockedAfterReachingSkill()) +
							TEXT(" intelligence to unlock this door!")));

						break;

					case EDoorUnlocked::EEverySkill:

						HowToUnlockDoorText->SetText(FText::FromString(TEXT("You need ") +
							FString::FromInt((*doorIt)->GetUnlockedAfterReachingSkill()) +
							TEXT(" in every skill to unlock this door!")));

						break;
					}

					break;
				}

				else if (doorIt->GetDoorOpening() && player->IsDoorNearby(*doorIt))
				{
					HowToUnlockDoorText->SetText(FText::FromString(TEXT("Wait for the door to open!")));
					break;
				}

				else continue;
			}
		}
	}
}

void UHowToUnlockDoorWidget::NativeDestruct()
{
	if (InstructionText)
	{
		InstructionText->RemoveFromParent();
		InstructionText = nullptr;
	}

	if (HowToUnlockDoorText)
	{
		HowToUnlockDoorText->RemoveFromParent();
		HowToUnlockDoorText = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (HowToUnlockDoorOverlay)
	{
		HowToUnlockDoorOverlay->RemoveFromParent();
		HowToUnlockDoorOverlay = nullptr;
	}
}

void UHowToUnlockDoorWidget::SetInstructionText(FKey& FKey_)
{
	if (InstructionText)
	{
		InstructionText->SetText(FText::FromString(TEXT("Press ") + FKey_.GetDisplayName().ToString() +
			TEXT(" to continue!")));
	}
}
