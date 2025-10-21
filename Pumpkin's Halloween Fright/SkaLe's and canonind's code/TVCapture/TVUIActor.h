// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TVUIActor.generated.h"

class UStaticMeshComponent;
class AUIManager;
class UUIMeshInteractor;
class UMaterialInstanceDynamic;
class UAudioComponent;

/*
* Actor that sets up a mesh with UI rendering and interaction
*/
UCLASS()
class GDTV_HALLOWEEN2025_API ATVUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATVUIActor();
	virtual void Tick(float DeltaTime) override;

	void TurnOn();
	void TurnOff();
	void AddStaticImage();
	void RemoveStaticImage();
	void AddStaticWhiteSound();
	void AddStaticVHSSound();
	void RemoveStaticSound();

protected:
	virtual void BeginPlay() override;

	void UpdateTVSound(USoundBase* NewSound);
	void FindUIManager();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GlassMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUIMeshInteractor> MeshInteractor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> TVSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> TVStaticSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> VHSSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	TObjectPtr<AUIManager> UIManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	FName TextureParameterName = TEXT("UITexture");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	bool bAutoFindUIManager = true;

	UPROPERTY(EditDefaultsOnly, Category = "Screen")
	TObjectPtr<UMaterial> TurnedOffMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Screen")
	UTextureRenderTarget2D* WorldRenderTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Screen")
	UTextureRenderTarget2D* UIRenderTarget;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenDynamicMaterial = nullptr;

};
