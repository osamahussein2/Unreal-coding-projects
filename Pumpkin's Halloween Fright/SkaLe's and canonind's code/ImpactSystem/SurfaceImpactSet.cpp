// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactSystem/SurfaceImpactSet.h"

bool USurfaceImpactSet::GetImpactEffect(ESurfaceImpactType ImpactType, FSurfaceImpactEffect& OutEffect) const
{
	if (const FSurfaceImpactEffect* Effect = ImpactEffects.Find(ImpactType))
	{
		OutEffect = *Effect;
		return Effect->IsValid();
	}
	return false;
}

bool USurfaceImpactSet::HasImpactEffect(ESurfaceImpactType ImpactType) const
{
	if (const FSurfaceImpactEffect* Effect = ImpactEffects.Find(ImpactType))
	{
		return Effect->IsValid();
	}
	return false;
}
