// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/NerveActor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// Sets default values
ANerveActor::ANerveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANerveActor::BeginPlay()
{
	Super::BeginPlay();
	
	//OnSequenceSuccessful.AddUniqueDynamic(this, &ANerveActor::SetNerveSprite);
}

// Called every frame
/*void ANerveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

/*void ANerveActor::SetNerveSprite(int correctSequences_, int index_)
{
	spriteComponent->SetWorldScale3D(nerveScale);

	switch (correctSequences_)
	{
	case 1: // Completed the blue nerve sequence
		if (blueNerveSprites[index_]) spriteComponent->SetSprite(blueNerveSprites[index_]);
		break;

	case 2: // Completed the orange nerve sequence
		if (orangeNerveSprites[index_]) spriteComponent->SetSprite(orangeNerveSprites[index_]);
		break;

	case 3: // Completed the purple nerve sequence
		if (purpleNerveSprites[index_]) spriteComponent->SetSprite(purpleNerveSprites[index_]);
		break;

	case 4: // Completed the yellow nerve sequence
		if (yellowNerveSprites[index_]) spriteComponent->SetSprite(yellowNerveSprites[index_]);
		break;
	}

	if (OnSequenceSuccessful.IsBound()) OnSequenceSuccessful.RemoveDynamic(this, &ANerveActor::SetNerveSprite);
}*/

/*void ANerveActor::UnbindDelegates()
{
	if (OnSequenceSuccessful.IsBound()) OnSequenceSuccessful.RemoveDynamic(this, &ANerveActor::SetNerveSprite);
}*/