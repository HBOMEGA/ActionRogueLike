// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category= "CameraShake")
	TSubclassOf<UCameraShakeBase> Shake_Impact;
	
	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	UParticleSystem* ImpactVfx;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* FlightSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category= "CameraShake")
	float InnerRadius;

	UPROPERTY(EditAnywhere, Category= "CameraShake")
	float OuterRadius;
	

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult & Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;
	
public:	

	// Sets default values for this actor's properties
	ASBaseProjectile();

};
