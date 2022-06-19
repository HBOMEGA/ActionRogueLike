// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"


#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	
	HandSocketName = TEXT("Muzzle_01");

	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character )
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached( CastingEffect, Character->GetMesh(), HandSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget
		);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("AttackDelay_Elapsed"), Character );

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false );
	}

	

	
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		float SweepRadius = 20.0f;
		int32 SweepDistanceFallback = 5000;
		
		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		
		// FVector Location;
		// FRotator Rotation;
		// GetController()->GetPlayerViewPoint(Location, Rotation) ;    
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + SweepRadius;
		
		FVector TraceEnd =  TraceStart + ( InstigatorCharacter->GetControlRotation().Vector() * SweepDistanceFallback);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// find new direction/location from hand pointing to impact point in world
		FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();
		
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);		
		
		GetWorld()->SpawnActor<AActor>( ProjectileClass,  SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}
