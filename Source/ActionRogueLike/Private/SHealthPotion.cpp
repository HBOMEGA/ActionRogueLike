// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


ASHealthPotion::ASHealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	MeshComp->SetupAttachment(RootComponent);

	CreditValue = -5;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
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
	
	USAttributeComponent* AttribComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttribComp) && !AttribComp->IsFullHealth())
	{
		if ( AttribComp->ApplyHealthChanges(InstigatorPawn,AttribComp->GetMaxHealth()))
        {
            HideAndCoolDownPickUp();
			PlayerState->ApplyCreditChange(InstigatorPawn, CreditValue );
		}					
	}
	
}


