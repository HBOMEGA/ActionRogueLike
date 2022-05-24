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

public:

	ASTeleportProjectile();

protected:

	UPROPERTY(EditAnywhere, Category= "Attack")
	class UParticleSystem* ExplodeParticle;	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();
	
	void TeleportPlayer();


	FTimerHandle TimerHandle_Explode;

	UPROPERTY(EditAnywhere)
	float ExplodeRate = 0.2f;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnActorHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	
};
