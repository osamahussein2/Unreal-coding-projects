// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentDayStatsWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UWidgetAnimation;

UENUM()
enum class DayStatsTransitionState : uint8
{
	ENull,
	EContinue
};

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UCurrentDayStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCurrentDayStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

	UFUNCTION()
	void Continue();

private:
	void CreateCustomerNumberTexts(class APortfolioBuilders_GM* gameMode_);
	void CreateCustomerScoreTexts(class APortfolioBuilders_GM* gameMode_);

	void CreateAverageScoreTexts(class APortfolioBuilders_GM* gameMode_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CurrentDayStatsCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentDayStatsTitleText = nullptr;

	// Text objects that don't need to be binded to the widget blueprint
	UPROPERTY(VisibleAnywhere)
	TArray<UTextBlock*> CurrentDayCustomerNumberTexts;

	UPROPERTY(VisibleAnywhere)
	TArray<UTextBlock*> CurrentDayCustomerScoreTexts;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ContinueButton = nullptr;

	/*UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	ETextJustify::Type customerNumberTextJustify = ETextJustify::Type::Right;*/

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	FVector2D customerNumberTextsLocation = FVector2D(-100.0f, -200.0f);

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	FVector2D customerScoreTextsLocation = FVector2D(100.0f, -200.0f);

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	FVector2D averageScoreTextsLocation = FVector2D(0.0f, -200.0f);

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	float customerStatsFontSize = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	float customerStatsPaddingY = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Text Initialization Parameters")
	float averageStatsPaddingY = 70.0f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* ContinueBurgerFlipAnimation = nullptr;

	DayStatsTransitionState dayStatsTransitionState = DayStatsTransitionState::ENull;
};
