// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerInfoWidget.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UPlayerInfoWidget::UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), typedName()
{
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Let's update the hint text to the tell the player to type a name
	if (nameTextBox)
	{
		nameTextBox->SetHintText(FText::FromString("Type your name in here!"));

		nameTextBox->OnTextChanged.AddUniqueDynamic(this, &UPlayerInfoWidget::OnTextChanged);
		nameTextBox->OnTextCommitted.AddUniqueDynamic(this, &UPlayerInfoWidget::OnTextCommitted);
	}
}

void UPlayerInfoWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}

void UPlayerInfoWidget::SetPlayerName(FText playerName_)
{
	if (PlayerName)
	{
		TArray<AActor*> foundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

		TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

		foundPlayer[0] = nullptr;

		if (playerCharacter.IsValid())
		{
			PlayerName->SetText(playerName_);
			playerCharacter = nullptr;
		}
	}
}

void UPlayerInfoWidget::SetPlayerScore(int playerScore_)
{
	if (PlayerScore)
	{
		TArray<AActor*> foundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

		TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

		foundPlayer[0] = nullptr;

		if (playerCharacter.IsValid())
		{
			PlayerScore->SetText(FText::FromString(FString::FromInt(playerScore_)));
			playerCharacter = nullptr;
		}
	}
}

void UPlayerInfoWidget::SetLeaderboardButtons(UButton* backButton_, UButton* resetButton_, UImage* leftBackImage_, 
	UImage* rightBackImage_, UImage* leftResetImage_, UImage* rightResetImage_)
{
	if (!backButton_ || !resetButton_ || !leftBackImage_ || !rightBackImage_ || !leftResetImage_ || !rightResetImage_) return;

	if (!backButton) backButton = backButton_;
	if (!resetButton) resetButton = resetButton_;

	if (!leftBackImage) leftBackImage = leftBackImage_;
	if (!rightBackImage) rightBackImage = rightBackImage_;

	if (!leftResetImage) leftResetImage = leftResetImage_;
	if (!rightResetImage) rightResetImage = rightResetImage_;
}

void UPlayerInfoWidget::ShowLeaderboardButtons()
{
	if (!backButton || !resetButton || !leftBackImage || !rightBackImage || !leftResetImage || !rightResetImage) return;

	backButton->SetVisibility(ESlateVisibility::Visible);
	resetButton->SetVisibility(ESlateVisibility::Visible);

	leftBackImage->SetVisibility(ESlateVisibility::Visible);
	rightBackImage->SetVisibility(ESlateVisibility::Visible);

	if (leftResetImage->IsVisible()) leftResetImage->SetVisibility(ESlateVisibility::Hidden);
	if (rightResetImage->IsVisible()) rightResetImage->SetVisibility(ESlateVisibility::Hidden);

	backButton = nullptr;
	resetButton = nullptr;
	leftBackImage = nullptr;
	rightBackImage = nullptr;
	leftResetImage = nullptr;
	rightResetImage = nullptr;
}

void UPlayerInfoWidget::ShowEditableText()
{
	if (BorderC) BorderC->SetVisibility(ESlateVisibility::Hidden);
	if (HorizontalBox) HorizontalBox->SetVisibility(ESlateVisibility::Hidden);

	if (nameTextBox) nameTextBox->SetVisibility(ESlateVisibility::Visible);
	if (OverlayD) OverlayD->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerInfoWidget::ShowActualLeaderboard()
{
	if (BorderC) BorderC->SetVisibility(ESlateVisibility::Visible);
	if (HorizontalBox) HorizontalBox->SetVisibility(ESlateVisibility::Visible);

	if (nameTextBox)
	{
		nameTextBox->SetVisibility(ESlateVisibility::Hidden);
		nameTextBox->ConditionalBeginDestroy();
		nameTextBox = nullptr;
	}

	if (OverlayD)
	{
		OverlayD->SetVisibility(ESlateVisibility::Hidden);
		OverlayD->ConditionalBeginDestroy();
		OverlayD = nullptr;
	}
}

void UPlayerInfoWidget::OnTextChanged(const FText& Text)
{
	typedName = Text;
}

void UPlayerInfoWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	// Don't continue if either there is no player name, player score, player didn't press enter and the typed name isn't an empty string
	if (!PlayerName || !PlayerScore || CommitMethod != ETextCommit::Type::OnEnter || typedName.IsEmpty() ||
		!InputFailedText || !nameTextBox)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No player information is found!"));
		return;
	}

	// Play the input failed text animation once the typed name string exceeds the max typed name string length
	else if (typedName.ToString().Len() > maxTypedNameStringLength)
	{
		if (!IsAnimationPlaying(InputFailedText)) PlayAnimation(InputFailedText);
		return;
	}

	else if (playerCharacter.IsValid() && !IsAnimationPlaying(InputFailedText))
	{
		for (int i = 0; i < playerCharacter->GetPlayerLeaderboardStatsSize(); i++)
		{
			if (!playerCharacter->GetNamesReceived(i))
			{
				playerCharacter->SetPlayerName(typedName, i);
				PlayerName->SetText(playerCharacter->GetPlayerNames(i));

				PlayerScore->SetText(FText::FromString(FString::FromInt(playerCharacter->GetPlayerScores(i))));
			}
		}

		ShowLeaderboardButtons();
		ShowActualLeaderboard();

		playerCharacter = nullptr;
	}

	if (nameTextBox)
	{
		nameTextBox->OnTextChanged.RemoveDynamic(this, &UPlayerInfoWidget::OnTextChanged);
		nameTextBox->OnTextCommitted.RemoveDynamic(this, &UPlayerInfoWidget::OnTextCommitted);
	}
}