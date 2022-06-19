// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Engine/ICookInfo.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributeComponent::KillEm(AActor* InstigatorActor)
{
	return ApplyHealthChanges(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::bIsAlive() const
{
	return  Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChanges( AActor* InstigatorActor, float Delta)
{
	if ( !GetOwner()->CanBeDamaged() && Delta <= 0.0f )
	{
		return false;
	}

	if (Delta < 0.0f )
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;	
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;	

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, MaxHealth, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f )
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled( GetOwner(), InstigatorActor );
		}
	}

	return ActualDelta != 0;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor )
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes( Actor );
	if ( AttributeComp )
	{
		return AttributeComp->bIsAlive();
	}
	return false;
}
