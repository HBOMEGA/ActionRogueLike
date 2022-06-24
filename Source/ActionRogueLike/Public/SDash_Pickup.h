// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickup.h"
#include "SDash_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDash_Pickup : public ASBasePickup
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;


public:
	ASDash_Pickup();

protected:

	UPROPERTY(EditAnywhere, Category= "Action")
	TSubclassOf<class USAction> GrantAction;
	
};
