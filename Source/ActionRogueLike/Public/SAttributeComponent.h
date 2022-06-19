// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams( FOnHealthChanged, AActor*, InstigatedActor,class USAttributeComponent*, OwningComp, float, NewHealth, float, MaximumHealth, float, Delta);

static TAutoConsoleVariable<float>CVarDamageMultiplier( TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Multiplier for Attribute Component!"), ECVF_Cheat );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category= "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category= "Attributes", meta= ( DisplayName = "IsAlive"))
	static bool IsActorAlive( AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attributes")
	float MaxHealth;

	
	
public:
	UFUNCTION(BlueprintCallable)
	bool KillEm(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool bIsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;
	
	UFUNCTION(BlueprintCallable, Category= "Attributes")
	bool ApplyHealthChanges( AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
		
};
