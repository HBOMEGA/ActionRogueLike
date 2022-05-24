// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	ExplodeParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Explode Particle"));
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnActorHit );	
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASTeleportProjectile::Explode, ExplodeRate );
}

void ASTeleportProjectile::Explode()
{
	MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation(), GetActorRotation());
	
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASTeleportProjectile::TeleportPlayer, ExplodeRate );
	
}

void ASTeleportProjectile::TeleportPlayer()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
	Destroy();
}

void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ASTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	MovementComp->StopMovementImmediately();
	if ( Hit.GetActor())
	{
		Explode();
	}
	
}
