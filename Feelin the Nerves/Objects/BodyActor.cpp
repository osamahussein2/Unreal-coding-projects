// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/BodyActor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// Sets default values
ABodyActor::ABodyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABodyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
/*void ABodyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

void ABodyActor::ResetSprite()
{
	if (spriteComponent && bodySprites[0] && spriteComponent->GetSprite() != bodySprites[0])
		spriteComponent->SetSprite(bodySprites[0]);
}

void ABodyActor::ShowBodyNervesSprite(int& bodyIndex_)
{
	if (spriteComponent && bodySprites[bodyIndex_] && spriteComponent->GetSprite() != bodySprites[bodyIndex_])
		spriteComponent->SetSprite(bodySprites[bodyIndex_]);

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ABodyActor::ResetSprite, timeToResetSprite, false);

	if (OnSequenceSuccessful.IsBound())
		OnSequenceSuccessful.RemoveDynamic(this, &ABodyActor::ShowBodyNervesSprite);
}

void ABodyActor::BindOnSequenceSuccessful()
{
	OnSequenceSuccessful.AddUniqueDynamic(this, &ABodyActor::ShowBodyNervesSprite);
}