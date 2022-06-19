// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasePickup.h"

#include "Components/SphereComponent.h"


// Sets default values
ASBasePickup::ASBasePickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComp->SetCollisionProfileName(TEXT("PickUp"));
	RootComponent = SphereComp;

	RespawnRate = 10.0f;

}

void ASBasePickup::SetPickUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive, true);
}

void ASBasePickup::ShowPickUp()
{
	SetPickUpState(true);
}

void ASBasePickup::HideAndCoolDownPickUp()
{
	SetPickUpState(false);
	
	FTimerHandle TimerHandle_HealthRespawn;
	GetWorldTimerManager().SetTimer(TimerHandle_HealthRespawn, this, &ASBasePickup::ShowPickUp, RespawnRate );
}

void ASBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
	// ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}


