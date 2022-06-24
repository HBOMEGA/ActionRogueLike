// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SBasePickup.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBasePickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category= "Credits")
	int32 CreditValue;
	
	UPROPERTY(VisibleAnywhere, Category= "Respawn")
	float RespawnRate;

	UFUNCTION()
	void ShowPickUp();

	void HideAndCoolDownPickUp();

	void SetPickUpState( bool bNewIsActive );

	UPROPERTY(VisibleAnywhere, Category= "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	// Sets default values for this actor's properties
	ASBasePickup();

};
