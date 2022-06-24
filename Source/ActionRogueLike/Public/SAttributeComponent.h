// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams( FOnHealthChanged, AActor*, InstigatedActor,class USAttributeComponent*, OwningComp, float, NewHealth, float, MaximumHealth, float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams( FOnRageChanged, AActor*, InstigatedActor,class USAttributeComponent*, OwningComp, float, NewRage, float, MaximumRage, float, Delta);

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

	//Health Attrib
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attributes")
	float MaxHealth;

	
	// Rage Attrib
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attributes")
	float MaxRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attributes")
	float RagePercentage;

	
	
public:
	
	UFUNCTION(BlueprintCallable)
	bool KillEm(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool bIsAlive() const;

	// Health Attrib	
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


	// Rage Attribs
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category= "Attributes")
	bool AddRage( AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category= "Attributes")
	bool RemoveRage( AActor* InstigatorActor, float Delta);

	// UFUNCTION(BlueprintCallable)
	// bool IsFullRage() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	// UFUNCTION(BlueprintCallable)
	// float GetMaxRage() const;
		
};
