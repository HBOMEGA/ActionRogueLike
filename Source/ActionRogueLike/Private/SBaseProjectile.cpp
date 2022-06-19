// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
 	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));	
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionProfileName( TEXT("Projectile"));	
	
	SphereComp->IgnoreActorWhenMoving( GetInstigator(), true);
	RootComponent = SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect Component"));	
	EffectComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComp->InitialSpeed= 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	FlightSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FlightAudio Comp"));
	FlightSound->SetupAttachment(SphereComp);

	ImpactSound = CreateDefaultSubobject<USoundBase>(TEXT("ImpactAudio Comp"));

	InnerRadius = 0.0;
	OuterRadius = 0.0;
}

void ASBaseProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Explode!"));
	Explode();
}

void ASBaseProjectile::Explode_Implementation()
{
	if (ensure( IsValid(this) ) )
	{		
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 12, FColor::Red, false, 2.0f );
		
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation() );

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation() );
		
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake_Impact, GetActorLocation(), InnerRadius, OuterRadius );
		
		Destroy();
	}
}

void ASBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASBaseProjectile::OnActorHit);	
	
}






