// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditCoin.h"

#include "SPlayerState.h"


ASCreditCoin::ASCreditCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Comp"));
	MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	MeshComp->SetupAttachment(RootComponent);

	CreditValue = 1;
}

void ASCreditCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	
	if ( ASPlayerState* PlayerState = InstigatorPawn->GetInstigatorController()->GetPlayerState<ASPlayerState>() )
	{
		PlayerState->ApplyCreditChange(InstigatorPawn, CreditValue );
		HideAndCoolDownPickUp();
	}
}
