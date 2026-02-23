// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

class USceneCaptureComponent2D;
class UPaperSpriteComponent;
class UMarkerWidget;

UENUM()
enum class NPCDialogueState : uint8
{
	NotGiven,
	Given,
	Completed,
	Finished
};

class UBoxComponent;

UCLASS()
class EHGAMEJAM_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	bool GetPlayerTriggerFound() const { return playerTriggerFound; }
	FString GetNPCName() const { return npcName; }

	FString GetDialogueText() const { return dialogueLines[dialogueIndex]; }
	int GetDialogueIndex() const { return dialogueIndex; }
	void SetDialogueIndex(int value_);

	int GetDialogueLinesCount() const { return dialogueLines.Num(); }

	void GoToNextDialogue(int incrementedValue_);
	void ResetDialogueIndex();

	NPCDialogueState GetNPCDialogueState() const { return npcDialogueState; }
	void SetNPCDialogueState(NPCDialogueState newValue_) { npcDialogueState = newValue_; }

	int DialogueNotGivenDone() const { return transitionToGivenDialogue; }
	int DialogueGivenDone() const { return transitionToCompletedDialogue; }

	float GetTypingSpeed() const { return typeSpeed[dialogueIndex]; }

	void SpawnInstructionWidget();

	UFUNCTION()
	void RotateInstructionWidget();

	void DestroyInstructionWidget();

	void SpawnMarkerWidget();
	void DestroyMarkerWidget();

private:

	UFUNCTION()
	void UpdateMarkerWidget();

private:
	bool playerTriggerFound = false;

	UPROPERTY(EditAnywhere, Category = "NPC information")
	FString npcName;

	int dialogueIndex;

	UPROPERTY(EditAnywhere, Category = "NPC information")
	TArray<float> typeSpeed;

	UPROPERTY(EditAnywhere, Category = "NPC information")
	TArray<FString> dialogueLines;

	UPROPERTY(VisibleAnywhere, Category = "NPC information")
	NPCDialogueState npcDialogueState;

	UPROPERTY(EditAnywhere, Category = "NPC information")
	int transitionToGivenDialogue;

	UPROPERTY(EditAnywhere, Category = "NPC information")
	int transitionToCompletedDialogue;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* cubeMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* boxComponent;

	// Input Widget
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UWidgetComponent> widgetComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> inputWidgetClass;

	// Instruction widget spawn parameters
	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector widgetComponentSpawnLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FRotator widgetComponentSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector widgetComponentSpawnScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector2D widgetComponentDrawSize = FVector2D(500.0f, 500.0f);

	FTimerHandle rotateWidgetHandle;

	FRotator initialRotator = FRotator::ZeroRotator;

	// Minimap for NPC icon
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> sceneCaptureComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> spriteComponent;

	// Marker Widget
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UWidgetComponent> markerWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> markerWidgetClass;

	UPROPERTY()
	UMarkerWidget* markerWidget = nullptr;

	// Marker widget spawn parameters
	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector markerWidgetSpawnLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FRotator markerWidgetSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector markerWidgetSpawnScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector2D markerWidgetDrawSize = FVector2D(500.0f, 500.0f);

	FTimerHandle updateDistanceHandle;
};
