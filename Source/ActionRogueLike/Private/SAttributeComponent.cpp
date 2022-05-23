// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
}

bool USAttributeComponent::ApplyHealthChanges(float Delta)
{
	Health += Delta;

	return true;
}


