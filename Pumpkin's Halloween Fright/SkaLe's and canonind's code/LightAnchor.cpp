// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAnchor.h"
#include "Components/SphereComponent.h"

ALightAnchor::ALightAnchor()
{
	LightSphereOuter = CreateDefaultSubobject<USphereComponent>(TEXT("Anchor Outer"));
	SetRootComponent(LightSphereOuter);

	LightSphereOuter->SetSphereRadius(500.f);
	LightSphereOuter->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LightSphereInner = CreateDefaultSubobject<USphereComponent>(TEXT("Anchor Inner"));
	LightSphereInner->SetupAttachment(LightSphereOuter);
	LightSphereInner->SetSphereRadius(100.f);
	LightSphereInner->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LightRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Light Radius"));
	LightRadius->SetupAttachment(LightSphereOuter);
	LightRadius->SetSphereRadius(1000.f);
	LightRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ALightAnchor::GetAnchorPosition()
{
	return GetActorLocation();
}

float ALightAnchor::GetOuterRadius()
{
	return LightSphereOuter->GetScaledSphereRadius();
}

float ALightAnchor::GetInnerRadius()
{
	return LightSphereInner->GetScaledSphereRadius();
}

float ALightAnchor::GetLightRadius()
{
	return LightRadius->GetScaledSphereRadius();
}

