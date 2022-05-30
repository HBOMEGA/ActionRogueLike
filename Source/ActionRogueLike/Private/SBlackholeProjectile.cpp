// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile()
{
	
	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("Force Component"));
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->SetAutoActivate(true);
	ForceComp->Radius = 1000.0f;
	ForceComp->ForceStrength = -2000000.0f;

	// Optional Ignores 'Mass' of other objects, (if false will take objects mass into consideration
	ForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to effects, excluding WorldDynamic
	//ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	MovementComp->InitialSpeed= 1300.0f;
	MovementComp->ProjectileGravityScale = -0.2f;
	
}
