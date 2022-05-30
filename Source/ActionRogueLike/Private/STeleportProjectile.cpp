// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"


#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MovementComp->InitialSpeed = 6000.0f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASTeleportProjectile::Explode, DetonateDelay );
}

void ASTeleportProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
	
	EffectComp->DeactivateSystem();
	
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_TeleportDelay;
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &ASTeleportProjectile::TeleportInstigator, TeleportDelay );
}


void ASTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
		
		if (!ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation()) )
		{
			FCollisionShape Shape;
			Shape.SetCapsule(34.0f, 88.0f);
			
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
			ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
			
			FVector NewLocation = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

			FVector TraceEnd = NewLocation + -ActorToTeleport->GetActorUpVector() * 5000;

			FHitResult Hit;

			if ( GetWorld()->SweepSingleByObjectType(Hit, NewLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, QueryParams))
			{
				TraceEnd = Hit.ImpactPoint;
				ActorToTeleport->TeleportTo( TraceEnd, ActorToTeleport->GetActorRotation());
				
			}
		}
	}	
}