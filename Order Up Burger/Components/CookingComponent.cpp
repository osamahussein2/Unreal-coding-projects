// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CookingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Trigger/CookTrigger.h"
#include "Camera/CameraComponent.h"
#include "Objects/Bun.h"
#include "Objects/Burger.h"

// Sets default values for this component's properties
UCookingComponent::UCookingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCookingComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCookingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCookingComponent::ToggleCookingState(EPlayerState& playerState_, bool& overlayVisibility_,
	FVector& cameraLocation_, FRotator& cameraRotation_, AActor* actor_, FVector& characterLocation_,
	APlayerController* playerController_, FName socketName_)
{
	if (!actor_ || !playerController_) return;

	switch (playerState_)
	{
	case EPlayerState::EFreeRoam: // If the player is free roam
	{
		if (!cineCamera)
		{
			// Spawn the cine camera
			cineCamera = GetWorld()->SpawnActor<ACineCameraActor>(ACineCameraActor::StaticClass(), cameraLocation_,
				cameraRotation_, FActorSpawnParameters());

			// Don't worry about aspect ratio constraint
			cineCamera->GetCameraComponent()->SetConstraintAspectRatio(false);

			// Set the player actor location to be at the cooking location and control rotation to 0 on all axes
			actor_->SetActorLocation(characterLocation_);
			playerController_->SetControlRotation(FRotator::ZeroRotator);

			// Set the view target from the player controller class to be the spawned cine camera
			playerController_->SetViewTargetWithBlend(cineCamera);

			playerState_ = EPlayerState::ECooking; // Start cooking

			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				gameMode->HideCustomerOrderOverlay();
				gameMode->InvalidateTouchInterface();

				if (overlayVisibility_) overlayVisibility_ = false;

				gameMode->DestroyInstructionWidget();

				if (gameMode->GetMobileWidget())
				{
					gameMode->GetMobileWidget()->HideInteractionButton();
					gameMode->GetMobileWidget()->HideDropFoodButton();
				}

				gameMode->InitializePickFoodWidget();
				gameMode->InitializePickFoodMobileWidget();

				gameMode->SpawnBottomBun();
			}
		}

		break;
	}

	case EPlayerState::ECooking: // If the player is cooking
	{
		// If cine camera is valid, destroy it and set it to nullptr afterwards
		if (cineCamera)
		{
			// Also show the instruction to cook again if needed
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				gameMode->DestroyPickFoodWidget();
				gameMode->DestroyPickFoodMobileWidget();

				gameMode->SetTouchInterface();

				gameMode->InitializeInstructionWidget("Press SPACE to make meal");

				if (gameMode->GetMobileWidget())
				{
					gameMode->GetMobileWidget()->ShowInteractionButton();
					gameMode->GetMobileWidget()->ShowDropFoodButton();

					gameMode->GetMobileWidget()->SetInteractionText("Make meal");
					gameMode->GetMobileWidget()->SetDropFoodText("Hide/Show\nOrder");
				}
			}

			playerState_ = EPlayerState::EFreeRoam; // Go back to free roam state

			cineCamera->Destroy();
			cineCamera = nullptr;
		}

		break;
	}

	case EPlayerState::ECarryingFood: // If the player is carrying food with them
	{
		// If cine camera is valid, destroy it and set it to nullptr afterwards
		if (cineCamera)
		{
			// Also show the instruction to cook again if needed
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				// Also destroy the pick food widget
				gameMode->DestroyPickFoodWidget();
				gameMode->DestroyPickFoodMobileWidget();
				gameMode->DestroyInstructionWidget();

				gameMode->SetTouchInterface();

				if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideInteractionButton();
			}

			cineCamera->Destroy();
			cineCamera = nullptr;
		}

		if (APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner()))
		{
			AttachFoodOnOwner(player->GetMesh(), player->GetFoodAttachOffsetZ(), player, socketName_);
			player->DestroyCookingComponent();
		}

		break;
	}

	default:
		break;
	}
}

void UCookingComponent::DestroyReferences()
{
	if (cineCamera)
	{
		cineCamera->Destroy();
		cineCamera = nullptr;
	}
}

void UCookingComponent::AttachFoodOnOwner(USkeletalMeshComponent* skeletalMeshComp_, float foodAttachOffsetZ_, 
	AActor* attachingActor_, FName& socketName_)
{
	if (!skeletalMeshComp_ || !attachingActor_) return;

	TArray<AActor*> buns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABun::StaticClass(), buns);

	TArray<AActor*> burgers; // Including toppings
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABurger::StaticClass(), burgers);

	for (AActor* bun : buns)
	{
		if (skeletalMeshComp_->DoesSocketExist(socketName_))
		{
			const FVector& definedSocketLocation = FVector(skeletalMeshComp_->GetSocketLocation(socketName_).X,
				skeletalMeshComp_->GetSocketLocation(socketName_).Y, foodAttachOffsetZ_ + bun->GetActorLocation().Z);

			bun->SetActorEnableCollision(false);
			bun->SetActorLocation(definedSocketLocation);
			bun->AttachToActor(attachingActor_, FAttachmentTransformRules::KeepWorldTransform, socketName_);
		}

		bun = nullptr;
	}

	for (AActor* burger : burgers)
	{
		if (skeletalMeshComp_->DoesSocketExist(socketName_))
		{
			const FVector& definedSocketLocation = FVector(skeletalMeshComp_->GetSocketLocation(socketName_).X,
				skeletalMeshComp_->GetSocketLocation(socketName_).Y, foodAttachOffsetZ_ + burger->GetActorLocation().Z);

			burger->SetActorEnableCollision(false);
			burger->SetActorLocation(definedSocketLocation);
			burger->AttachToActor(attachingActor_, FAttachmentTransformRules::KeepWorldTransform, socketName_);
		}

		burger = nullptr;
	}
}