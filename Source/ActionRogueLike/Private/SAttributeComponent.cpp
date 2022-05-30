// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
}

bool USAttributeComponent::bIsAlive() const
{
	return  Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChanges(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}


