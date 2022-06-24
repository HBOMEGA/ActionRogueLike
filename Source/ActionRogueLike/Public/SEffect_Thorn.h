// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SEffect_Thorn.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USEffect_Thorn : public USActionEffect
{
	GENERATED_BODY()

public:
	USEffect_Thorn();
	
	

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void StopAction_Implementation(AActor* InstigatorActor) override;



protected:
	UPROPERTY(EditDefaultsOnly, Category= "Effect")
	float ThornPercent;

	UFUNCTION()
	void OnThornBuff(AActor* InstigatorActor, class USAttributeComponent* OwningComp, float NewHealth, float MaximumHealth, float Delta );

};
