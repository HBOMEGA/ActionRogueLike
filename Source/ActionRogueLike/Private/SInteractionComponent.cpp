// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"


static TAutoConsoleVariable<bool>CVarDebugDrawInteraction( TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for the Interaction component"), ECVF_Cheat );



void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetInstigatorController()->GetPlayerViewPoint( EyeLocation, EyeRotation );

	FVector End = EyeLocation + (  EyeRotation.Vector() * 1000 ) ;
	
	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType( Hit, EyeLocation, End, ObjectQueryParams );

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere( Radius );
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType( Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if ( HitActor && HitActor->Implements<USGameplayInterface>() )
        {    		
        	APawn* MyPawn = Cast<APawn>( MyOwner );
        	ISGameplayInterface::Execute_Interact( HitActor, MyPawn);
			
        }

		if ( bDebugDraw )
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 5.0 );
		}	
		
		break;
	}
	
	if ( bDebugDraw )
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.0f, 0, 3.0f );
	}
	
}
