// Fill out your copyright notice in the Description page of Project Settings.


#include "TVCapture/TVUIActor.h"
#include "UI/UIManager.h"
#include "UI/UIMeshInteractor.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


ATVUIActor::ATVUIActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UIMesh"));
	ScreenMesh->SetupAttachment(DefaultRoot);

	ScreenMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ScreenMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ScreenMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ScreenMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMesh"));
	GlassMesh->SetupAttachment(DefaultRoot);
	GlassMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshInteractor = CreateDefaultSubobject<UUIMeshInteractor>(TEXT("UIInteractor"));

	TVSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TVSound"));
	TVSoundComponent->SetupAttachment(RootComponent);

	TVSoundComponent->bAutoActivate = false;
	TVSoundComponent->SetTickableWhenPaused(true);
}

void ATVUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bAutoFindUIManager && !UIManager)
	{
		FindUIManager();
	}

	if (MeshInteractor && UIManager)
	{
		MeshInteractor->TargetMesh = ScreenMesh;
		MeshInteractor->UIManager = UIManager;
	}

	UMaterialInterface* BaseMat = ScreenMesh->GetMaterial(0);
	ScreenDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);

	ScreenDynamicMaterial->SetTextureParameterValue(FName("WorldTexture"), WorldRenderTarget);
	ScreenDynamicMaterial->SetTextureParameterValue(FName("UITexture"), UIRenderTarget);

	FVector2D RTSize(WorldRenderTarget->SizeX, WorldRenderTarget->SizeY);
	ScreenDynamicMaterial->SetVectorParameterValue(FName("TextureSize"), FLinearColor(RTSize.X, RTSize.Y, 0, 0));

}


void ATVUIActor::UpdateTVSound(USoundBase* NewSound)
{
	if (!NewSound) return;
	if (TVSoundComponent->Sound == NewSound && TVSoundComponent->IsPlaying()) return;

	if (TVSoundComponent->IsPlaying())
	{
		TVSoundComponent->FadeOut(0.05f, 0.0f);
	}
	TVSoundComponent->SetSound(NewSound);
	TVSoundComponent->FadeIn(0.05f, 1.0f);
}


void ATVUIActor::FindUIManager()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		UIManager = Cast<AUIManager>(PC->GetHUD());
	}
}

void ATVUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATVUIActor::TurnOn()
{
	ScreenMesh->SetMaterial(0, ScreenDynamicMaterial);
}

void ATVUIActor::TurnOff()
{
	ScreenMesh->SetMaterial(0, TurnedOffMaterial);
}

void ATVUIActor::AddStaticImage()
{
	ScreenDynamicMaterial->SetScalarParameterValue(FName("StaticWeight"), 1.f);
}

void ATVUIActor::RemoveStaticImage()
{
	ScreenDynamicMaterial->SetScalarParameterValue(FName("StaticWeight"), 0.f);
}

void ATVUIActor::AddStaticWhiteSound()
{
	UpdateTVSound(TVStaticSound);
}

void ATVUIActor::AddStaticVHSSound()
{
	UpdateTVSound(VHSSound);
}

void ATVUIActor::RemoveStaticSound()
{
	if (TVSoundComponent->IsPlaying())
	{
		TVSoundComponent->FadeOut(0.05f, 0.0f);
	}
}

