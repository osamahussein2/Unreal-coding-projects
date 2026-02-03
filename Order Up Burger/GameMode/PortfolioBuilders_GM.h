// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/CustomerOrderWidget.h"
#include "Widgets/MobileWidget.h"
#include "PortfolioBuilders_GM.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API APortfolioBuilders_GM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APortfolioBuilders_GM();

	void SpawnCustomer();
	void AddCustomerInformation();
	void AddAverageScoreInformation();

	void RestartGame();

	// Widget related functions
	void InitializeInstructionWidget(FString text_);
	void DestroyInstructionWidget();

	void InitializePickFoodWidget();
	void DestroyPickFoodWidget();

	void InitializeCustomerOrderWidget();
	void HideCustomerOrderOverlay();
	void ShowCustomerOrderOverlay();
	void DestroyCustomerOrderWidget();

	void InitializeCustomerScoreWidget();
	void DestroyCustomerScoreWidget();

	void InitializeMainMenuWidget();
	void DestroyMainMenuWidget();

	void InitializeCurrentDayWidget(bool incrementDay_);
	void DestroyCurrentDayWidget();

	void InitializeCurrentDayStatsWidget();
	void DestroyCurrentDayStatsWidget();

	void InitializePauseMenuWidget();
	void DestroyPauseMenuWidget();
	void DestroyPauseMenuWidget(bool goingToMainMenu);

	void InitializeMobileWidget();
	void DestroyMobileWidget();

	void InitializePickFoodMobileWidget();
	void DestroyPickFoodMobileWidget();

	// Spawn food functions
	void SpawnBottomBun();
	void SpawnTopBun();

	void SpawnBurger();
	void SpawnTopping(FString& toppingName_);

	// Current day
	int GetCurrentDay() const { return currentDay; }
	int GetMaxDays() const { return maxDays; }

	bool AreAllDaysCompleted() const { return currentDay >= maxDays; }

	bool CompletedDay();
	void DestroySpawnedEntities();

	// Customer number and score array getters
	int GetCustomerNumberArrayLength() const { return customerNumbers.Num(); }
	int GetCustomerScoreArrayLength() const { return customerScores.Num(); }
	int GetAverageScoreArrayLength() const { return averageScores.Num(); }

	int GetCustomerNumber(int& index) const { return customerNumbers[index]; }
	int GetCustomerScore(int& index) const { return customerScores[index]; }
	int GetAverageScore(int& index) const { return averageScores[index]; }

	void IncrementAverageScore(int& value_) { averageScore += value_; }
	void DivideUpAverageScore(int dividedValue_) { averageScore /= dividedValue_; }

	// Customer order widget getter
	UCustomerOrderWidget* GetCustomerOrderWidget() const { return customerOrderWidget; }

	UMobileWidget* GetMobileWidget() const { return mobileWidget; }

	TSubclassOf<AActor> GetCustomerClass(int& index_) { return CustomerClass[index_]; }
	int GetCustomerClassLength() { return CustomerClass.Num(); }

	void SetTouchInterface();
	void InvalidateTouchInterface();

	// Bind this function to the delegate
	/*UFUNCTION()
	void WaitUntilToShowCurrentDayStats();*/

	UPROPERTY(VisibleAnywhere)
	int playerScore = 0;

protected:
	virtual void BeginPlay() override;

private:
	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> PlayerCharacterClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> CustomerClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class AChair>> chairs;

	int customerIndex = 0;
	int spawnPointIndex = 0;

	int customersSpawned = 0;

	// Widget classes to spawn/destroy
	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UInstructionWidget> instructionWidgetClass;

	class UInstructionWidget* instructionWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UPickFoodWidget> pickFoodWidgetClass;

	class UPickFoodWidget* pickFoodWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UCustomerOrderWidget> customerOrderWidgetClass;

	UCustomerOrderWidget* customerOrderWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UCustomerScoreWidget> customerScoreWidgetClass;

	class UCustomerScoreWidget* customerScoreWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UMainMenuWidget> mainMenuWidgetClass;

	class UMainMenuWidget* mainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UCurrentDayWidget> currentDayWidgetClass;

	class UCurrentDayWidget* currentDayWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UCurrentDayStatsWidget> currentDayStatsWidgetClass;

	class UCurrentDayStatsWidget* currentDayStatsWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UPauseMenuWidget> pauseMenuWidgetClass;

	class UPauseMenuWidget* pauseMenuWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UMobileWidget> mobileWidgetClass;

	class UMobileWidget* mobileWidget;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<class UPickFoodMobileWidget> pickFoodMobileWidgetClass;

	class UPickFoodMobileWidget* pickFoodMobileWidget;

	UPROPERTY(EditAnywhere, Category = "Food Objects")
	TArray<TSubclassOf<class ABun>> bunClass; // 0 for bottom bun, 1 for top bun

	UPROPERTY(EditAnywhere, Category = "Food Objects")
	TSubclassOf<class ABurger> burgerClass;

	UPROPERTY(EditAnywhere, Category = "Food Objects")
	TMap<FString, TSubclassOf<class ATopping>> toppingClass;

	UPROPERTY(EditAnywhere, Category = "Food Objects")
	FVector bunSpawnPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Food Objects")
	FVector foodSpawnPoint = FVector(790.408750, -103.158872, 330.0f);

	int currentDay = 0;

	TArray<int> customerNumbers;
	TArray<int> customerScores;

	int averageScore = 0;
	TArray<int> averageScores;

	UPROPERTY(EditAnywhere, Category = "Waiting Times")
	float timeUntilShowDayStats = 2.0f;

	FTimerHandle timerHandleToShowDayStats;

	UPROPERTY(EditAnywhere, Category = "Day Parameters")
	int maxDays = 4;

	// Mobile interface
	UPROPERTY(EditAnywhere, Category = "Mobile")
	class UTouchInterface* touchInterface;
};
