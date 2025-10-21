#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomRewardPad.generated.h"

class UBoxComponent;
class UArrowComponent;
class UMaterialInstanceDynamic;
class ALootSplitter;               // spitter
class ACandyCollectible;           // candy actor
class ARewardPadGroup;



UCLASS()
class GDTV_HALLOWEEN2025_API ARandomRewardPad : public AActor
{
    GENERATED_BODY()

public:
    ARandomRewardPad();

    UPROPERTY(EditInstanceOnly, Category="Puzzle")
    ARewardPadGroup* Group = nullptr;

    void SetPadColor(const FLinearColor& Color);
    

protected:
    virtual void BeginPlay() override;

    /** Visual pad */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pad")
    UStaticMeshComponent* PadMesh;

    /** Overlap trigger */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pad")
    UBoxComponent* Trigger;

    /** Where enemies spawn (local arrow you can rotate) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pad")
    UArrowComponent* SpawnArrow;

    /** Dynamic MID we drive */
    UPROPERTY(Transient)
    UMaterialInstanceDynamic* MID;

    /** Material settings */
    UPROPERTY(EditAnywhere, Category="Visual")
    int32 MaterialElementIndex = 0;

    /** Scalar intensity param (set to NAME_None to skip) */
    UPROPERTY(EditAnywhere, Category="Visual")
    FName EmissiveStrengthParam = TEXT("EmissiveStrength");

    /** Color param (set to NAME_None to skip) */
    UPROPERTY(EditAnywhere, Category="Visual")
    FName EmissiveColorParam = TEXT("EmissiveColor");

    UPROPERTY(EditAnywhere, Category="Visual")
    float EmissiveOn = 50.f;

    UPROPERTY(EditAnywhere, Category="Visual")
    float EmissiveOff = 1.f;

    UPROPERTY(EditAnywhere, Category="Visual")
    FLinearColor GreenColor = FLinearColor(0.1f, 1.0f, 0.2f);

    UPROPERTY(EditAnywhere, Category="Visual")
    FLinearColor RedColor   = FLinearColor(1.0f, 0.1f, 0.1f);

    UPROPERTY(EditAnywhere, Category="Visual")
    float FlashDuration = 0.6f;

    /** Logic */
    UPROPERTY(EditAnywhere, Category="Logic", meta=(ClampMin="0.0", ClampMax="1.0"))
    float GreenChance = 0.5f;

    /** If true, pad can only trigger once */
    UPROPERTY(EditAnywhere, Category="Logic")
    bool bOneShot = true;

    /** If not one-shot, time between triggers */
    UPROPERTY(EditAnywhere, Category="Logic", meta=(EditCondition="!bOneShot", ClampMin="0.0"))
    float CooldownSeconds = 1.0f;

    /** Hook to your existing spitter (optional, recommended) */
    UPROPERTY(EditInstanceOnly, Category="Spawns")
    ALootSplitter* LootSpitter = nullptr;

    /** Optional direct classes (used if LootSpitter is null for candy) */
    UPROPERTY(EditAnywhere, Category="Spawns")
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, Category="Spawns")
    TSubclassOf<ACandyCollectible> CandyClass;

    /** Offsets for direct spawning (when not using spitter) */
    UPROPERTY(EditAnywhere, Category="Spawns")
    FVector CandySpawnOffset = FVector(0,0,20);

    UPROPERTY(VisibleAnywhere, Category = "Spawns")
    FVector patrolRoute;

private:
    bool bLocked = false;
    double NextAllowedTime = 0.0;

    FTimerHandle FlashTimer;

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& Sweep);

    UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* Overlapped, AActor* Other,
                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

    void DoGreen();
    void DoRed();
    void SetPadVisual(const FLinearColor& Color, float Strength);
    void ResetPadVisual();
};
