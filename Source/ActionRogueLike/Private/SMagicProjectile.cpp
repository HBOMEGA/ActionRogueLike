// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include <Components/SphereComponent.h>
#include "SActionEffect.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	Damage = 20.0f;
    	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		 USActionComponent* ActionComp = Cast<USActionComponent>( OtherActor->GetComponentByClass(USActionComponent::StaticClass() ) );
		 if ( ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag) )
		 {
		 	MovementComp->Velocity = -MovementComp->Velocity;
		 	SetInstigator(Cast<APawn>(OtherActor));
			return;
		 }

		if ( USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult) )
		{
			if ( ActionComp )
			{
				ActionComp->AddAction(GetInstigator(), BurningEffectClass );
			}
			Explode();
		}
	}
}


