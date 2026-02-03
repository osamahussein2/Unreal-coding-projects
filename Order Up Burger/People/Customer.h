// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.generated.h"

UENUM()
enum class CustomerStatus
{
	ERequestingOrder UMETA(DisplayName = "Requesting Order"),
	EMadeOrder UMETA(DisplayName = "Made Order"),
	EOrderReceived UMETA(DisplayName = "Order Received")
};

UCLASS()
class PORTFOLIOBUILDERS_API ACustomer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomer();

	bool GetTriggerOverlapped() const { return triggerOverlapped; }
	CustomerStatus GetCustomerStatus() const { return customerStatus; }

	void TakeOrder();

	FString GetFoodOrdersElement() const { return foodOrders[mealIndex]; }

	FString GetFoodOrdersElementByValue(int index_) const { return foodOrders[index_]; }

	// Get the food orders element at a referenced index in memory
	FString GetFoodOrdersElement(int& index_) const { return foodOrders[index_]; }

	bool ContainsFoodElement(int index_, FString foodName_) const { return foodOrders[index_] == foodName_; }

	// Food order string getters
	FString GetBottomBun() const { return bottomBun; }
	FString GetBurger() const { return burger; }
	FString GetTomato() const { return tomato; }
	FString GetCheese() const { return cheese; }
	FString GetTopBun() const { return topBun; }

	int GetMealIndex() const { return mealIndex; }
	int GetRandomizedMealIndex() const { return randomizedMealIndex; }

	TArray<FString> GetFoodOrders() const { return foodOrders; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void LoopThroughMealOrderIndex();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> customerMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> boxMesh;

	bool triggerOverlapped = false;

	CustomerStatus customerStatus = CustomerStatus::ERequestingOrder;

	int mealIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Order Parameters")
	int minMealIndex = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Order Parameters")
	int maxMealIndex = 8;

	int randomizedMealIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Food Orders")
	TArray<FString> foodOrders;

	FString bottomBun = "BottomBun";
	FString burger = "Burger";
	FString tomato = "Tomato";
	FString cheese = "Cheese";
	FString topBun = "TopBun";
};
