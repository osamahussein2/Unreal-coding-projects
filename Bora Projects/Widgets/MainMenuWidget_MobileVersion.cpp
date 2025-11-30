// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget_MobileVersion.h"

UMainMenuWidget_MobileVersion::UMainMenuWidget_MobileVersion(const FObjectInitializer& ObjectInitializer) : 
    Super(ObjectInitializer)
{
}

void UMainMenuWidget_MobileVersion::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreditsMenuCanvas)
    {
        CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    }

    if (MainMenuCanvas)
    {
        MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
    }

    // Set up button delegates here
    PlayButton->OnPressed.AddDynamic(this, &UMainMenuWidget_MobileVersion::TransitionToPlayGame);
    CreditsButton->OnPressed.AddDynamic(this, &UMainMenuWidget_MobileVersion::TransitionToCreditsMenu);
    CreditsBackButton->OnPressed.AddDynamic(this, &UMainMenuWidget_MobileVersion::TransitionBackToMainMenu);
    QuitButton->OnPressed.AddDynamic(this, &UMainMenuWidget_MobileVersion::TransitionToQuitGame);

    if (ScrollCredits)
    {
        PlayAnimation(ScrollCredits);
        StopAnimation(ScrollCredits);
    }

    // Create the material instance from the material interface only if it's valid
    if (TVStaticMaterial)
    {
        materialInst = UMaterialInstanceDynamic::Create(TVStaticMaterial, this);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVCreditsStaticImage || !TVStaticImage || !materialInst) return;

    if (materialInst) materialInst->SetScalarParameterValue(TEXT("DarkenValue"), brightStaticValue);

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVCreditsStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance
}

void UMainMenuWidget_MobileVersion::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);

    // Loop the scrolling credits animation
    if (CreditsMenuCanvas->IsVisible() && ScrollCredits && !IsAnimationPlaying(ScrollCredits))
    {
        PlayAnimation(ScrollCredits, 0.0f, 1, EUMGSequencePlayMode::Forward, scrollCreditsAnimationSpeed);
    }
}

void UMainMenuWidget_MobileVersion::TransitionToPlayGame()
{
    if (PlayButton->IsVisible()) PlayButton->SetVisibility(ESlateVisibility::Hidden);
    if (CreditsButton->IsVisible()) CreditsButton->SetVisibility(ESlateVisibility::Hidden);
    if (QuitButton->IsVisible()) QuitButton->SetVisibility(ESlateVisibility::Hidden);
    if (WelcomeToTheGame->IsVisible()) WelcomeToTheGame->SetVisibility(ESlateVisibility::Hidden);

    // Make TV static darker
    if (TVStaticImage && materialInst)
    {
        materialInst->SetScalarParameterValue(TEXT("DarkenValue"), darkStaticValue);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVStaticImage || !materialInst) return;

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    FTimerHandle goBackToMainTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(goBackToMainTimerHandle, this,
        &UMainMenuWidget_MobileVersion::PlayLevel, goToNextStateTime, false);
}

void UMainMenuWidget_MobileVersion::TransitionToCreditsMenu()
{
    if (PlayButton->IsVisible()) PlayButton->SetVisibility(ESlateVisibility::Hidden);
    if (CreditsButton->IsVisible()) CreditsButton->SetVisibility(ESlateVisibility::Hidden);
    if (QuitButton->IsVisible()) QuitButton->SetVisibility(ESlateVisibility::Hidden);
    if (WelcomeToTheGame->IsVisible()) WelcomeToTheGame->SetVisibility(ESlateVisibility::Hidden);

    // Make TV static darker
    if (TVStaticImage && materialInst)
    {
        materialInst->SetScalarParameterValue(TEXT("DarkenValue"), darkStaticValue);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVStaticImage || !materialInst) return;

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    FTimerHandle goBackToMainTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(goBackToMainTimerHandle, this,
        &UMainMenuWidget_MobileVersion::CreditsMenu, goToNextStateTime, false);
}

void UMainMenuWidget_MobileVersion::TransitionToQuitGame()
{
    if (PlayButton->IsVisible()) PlayButton->SetVisibility(ESlateVisibility::Hidden);
    if (CreditsButton->IsVisible()) CreditsButton->SetVisibility(ESlateVisibility::Hidden);
    if (QuitButton->IsVisible()) QuitButton->SetVisibility(ESlateVisibility::Hidden);
    if (WelcomeToTheGame->IsVisible()) WelcomeToTheGame->SetVisibility(ESlateVisibility::Hidden);

    // Make TV static darker
    if (TVStaticImage && materialInst)
    {
        materialInst->SetScalarParameterValue(TEXT("DarkenValue"), darkStaticValue);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVStaticImage || !materialInst) return;

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    FTimerHandle goBackToMainTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(goBackToMainTimerHandle, this,
        &UMainMenuWidget_MobileVersion::QuitGame, goToNextStateTime, false);
}

void UMainMenuWidget_MobileVersion::TransitionBackToMainMenu()
{
    if (ScrollCredits && IsAnimationPlaying(ScrollCredits)) StopAnimation(ScrollCredits);

    if (CreditsBackButton->IsVisible()) CreditsBackButton->SetVisibility(ESlateVisibility::Hidden);

    if (!PlayButton->IsVisible()) PlayButton->SetVisibility(ESlateVisibility::Visible);
    if (!CreditsButton->IsVisible()) CreditsButton->SetVisibility(ESlateVisibility::Visible);
    if (!QuitButton->IsVisible()) QuitButton->SetVisibility(ESlateVisibility::Visible);
    if (!WelcomeToTheGame->IsVisible()) WelcomeToTheGame->SetVisibility(ESlateVisibility::Visible);

    // Make TV static darker
    if (TVCreditsStaticImage && materialInst)
    {
        materialInst->SetScalarParameterValue(TEXT("DarkenValue"), darkStaticValue);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVCreditsStaticImage || !materialInst) return;

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVCreditsStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    FTimerHandle goBackToMainTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(goBackToMainTimerHandle, this,
        &UMainMenuWidget_MobileVersion::GoToMain, goToNextStateTime, false);
}

void UMainMenuWidget_MobileVersion::PlayLevel()
{
    RemoveFromParent();

    // Open the game's level
    UGameplayStatics::OpenLevel(this, "/Game/Levels/GameLevel");
}

void UMainMenuWidget_MobileVersion::CreditsMenu()
{
    // Make TV static brighter
    if (TVStaticImage && materialInst)
    {
        materialInst->SetScalarParameterValue(TEXT("DarkenValue"), brightStaticValue);
    }

    // If we don't have a tv credits static image or a valid material instance, skip running the code below
    if (!TVStaticImage || !materialInst) return;

    FSlateBrush Brush;

    // Set the resource object from the brush to the material instance that was created earlier
    Brush.SetResourceObject(materialInst);
    TVStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

    if (!CreditsMenuCanvas->IsVisible())
    {
        if (!CreditsBackButton->IsVisible()) CreditsBackButton->SetVisibility(ESlateVisibility::Visible);

        MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
        CreditsMenuCanvas->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMainMenuWidget_MobileVersion::QuitGame()
{
    // Quit the game
    UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(),
        EQuitPreference::Quit, true);
}

void UMainMenuWidget_MobileVersion::GoToMain()
{
    if (!MainMenuCanvas->IsVisible())
    {
        // Make TV static brighter
        if (TVCreditsStaticImage && materialInst)
        {
            materialInst->SetScalarParameterValue(TEXT("DarkenValue"), brightStaticValue);
        }

        // If we don't have a tv credits static image or a valid material instance, skip running the code below
        if (!TVCreditsStaticImage || !materialInst) return;

        FSlateBrush Brush;

        // Set the resource object from the brush to the material instance that was created earlier
        Brush.SetResourceObject(materialInst);
        TVCreditsStaticImage->SetBrush(Brush); // Set the image to use the brush of the material instance

        if (ScrollCredits && IsAnimationPlaying(ScrollCredits)) StopAnimation(ScrollCredits);

        // Hide credits menu and show main menu
        CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
        MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
    }
}
