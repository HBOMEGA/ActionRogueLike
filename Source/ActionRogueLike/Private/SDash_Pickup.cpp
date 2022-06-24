// Fill out your copyright notice in the Description page of Project Settings.


#include "SDash_Pickup.h"

#include "SActionComponent.h"
#include "SPlayerState.h"


ASDash_Pickup::ASDash_Pickup()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	MeshComp->SetupAttachment(RootComponent);

	CreditValue = 0;
}


void ASDash_Pickup::Interact_Implementation(APawn* InstigatorPawn)
{
	// Super::Interact_Implementation(InstigatorPawn);
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	ASPlayerState* PlayerState = InstigatorPawn->GetInstigatorController()->GetPlayerState<ASPlayerState>();
	if ( PlayerState->GetPlayerCredit() + CreditValue < 0  )
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont Have Enough Credit To Pay For Potion! Potion Cost: %i, you have: %i!"), CreditValue, PlayerState->GetPlayerCredit() );
		return;
	}

	USActionComponent* ActComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()) );
	if ( ensure(ActComp) && !ActComp->DoesActionExist( GrantAction ))
	{
		ActComp->AddAction( InstigatorPawn, GrantAction );
	}

	
}