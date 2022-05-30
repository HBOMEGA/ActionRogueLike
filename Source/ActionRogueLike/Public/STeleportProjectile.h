// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category= "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category= "Teleport")
	float DetonateDelay;	
	
	FTimerHandle TimerHandle_DelayedDetonate;
	virtual void Explode_Implementation() override;
	
	void TeleportInstigator();

	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

	ASTeleportProjectile();
	
};
